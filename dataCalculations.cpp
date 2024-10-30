//
// Created by Yuyang Hu on 10/23/24.
//
#include "dataCalculations.h"

namespace dataCalculations {
    /**
     * Initalizes a new object of the trade structure
     * Creates a vector comprised of the closing prices for a given period
     * Sets the SMA, standard deviation, and the regression price
     * @param filename
     * @param risk - the percentage of the standard deviation we want the program to use to determine the decision
     */
    trade::trade(std::string filename, double risk) {
        this->risk = risk;
        data = read_file(filename, 4);
        currprice = data[data.size() - 1];
        sma = getSMA();
        deviation = getDeviation();
        regressionprice = getRegressionPrice(); //to be completed
    }
    /**
     * Computes and returns the average of the average of the data
     * @return double SMA
     */
    double trade::getSMA() {
        double sum = 0;
        for(double t : data) {
            sum += t;
        }
        return sum / (double) data.size();
    }
    /**
     * Computes the standard deviation of a given set of data
     * @return double deviation
     */
    double trade::getDeviation() {
        double sum = 0;
        for(double t : data) {
            sum += pow(t - sma, 2);
        }
        double n = (double) data.size();
        return sqrt(sum/(n - 1));
    }
    /**
     * Calculates the regression price at day n + 1
     * Where n is the number of days monitored
     * @return double regressionprice
     */
    double trade::getRegressionPrice() {
        Eigen::VectorXd prices(data.size());
        Eigen::VectorXd dates(data.size());
        for(long i = 0; i < data.size(); i++) {
            prices[i] = data[i];
            dates[i] = i + 1;
        }
        Eigen::Vector2d slopeIntercept = trendline(dates, prices);
        double regressionprice = slopeIntercept[0] * (data.size()) + slopeIntercept[1];
        return regressionprice;
    }
    /**
     * Takes in two x by 1 matrices and calculates the trendline using them
     * @param VectorXd x, comrpised of dates
     * @param VectorXd y, comprised of values
     * @return Vector2d composed of the slope in the first row, and the intercept in the second
     */
    Eigen::Vector2d trade::trendline(Eigen::VectorXd& x, Eigen::VectorXd& y) {
        Eigen::MatrixXd a(x.size(), 2);
        a.col(0) = x;
        a.col(1) = Eigen::VectorXd::Ones(x.size());
        Eigen::Vector2d slopeIntercept = (a.transpose() * a).ldlt().solve(a.transpose() * y);
        return slopeIntercept;
    }
    /**
     * Creates and returns a vector comprised of the data from a column in a .csv file
     * @param filename
     * @param column
     * @return vector of data
     */
    std::vector<double> trade::read_file(std::string filename, double column) {
        std::ifstream file(filename);
        if(filename.find(".csv") == std::string::npos) {
            throw std::invalid_argument("File does not exist");
        }
        if (!file.is_open()) {
            throw std::__1::ios_base::failure("Error, unable to open file"); // Exit with an error code
        }
        std::string line;
        std::vector<double> nums;
        std::getline(file, line);
        while(std::getline(file, line)) {
            std::string data = "";
            int space_count = 0;
            for(char c: line) {
                if(c == '\t') {
                    space_count++;
                    continue;
                }
                if(space_count == column - 1 && !isdigit(c) && c != '.') {
                    break;
                }
                if(space_count == column - 1) {
                    data += c;
                }
            }
            nums.push_back(std::stod(data));
        }
        return nums;
    }

    /**
     * Updates the trade values for a new file
     * @param filename
     */
    void trade::update(std::string filename) {
        data = read_file(filename, 4);
        currprice = data[data.size() - 1];
        sma = getSMA();
        deviation = getDeviation();
        regressionprice = getRegressionPrice();
    }




}