//
// Created by Yuyang Hu on 10/23/24.
//

#ifndef DATACALCULATIONS_H
#define DATACALCULATIONS_H
#include <Eigen/Dense>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

namespace dataCalculations {
    struct trade {
        //Constructor Declaration
        trade(std::string filename, double risk);

        // Function declarations
        double getSMA();
        double getDeviation();
        double getRegressionPrice();
        std::string getDecision();
        Eigen::Vector2d trendline(Eigen::VectorXd& x, Eigen::VectorXd& y);
        std::vector<double> read_file(std::string filename, double column);
        void update(std::string filename);

        // Variable Declarations
        std::vector<double> data;
        std::string filename;
        std::string decision;
        double sma;
        double currprice;
        double regressionprice;
        double deviation;
        double risk;

    };
}
#endif //DATACALCULATIONS_H


