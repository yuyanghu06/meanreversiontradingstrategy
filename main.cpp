#include "dataCalculations.h"
#include "tradeSession.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <boost/date_time/local_time/local_date_time.hpp>

namespace fs = std::filesystem;
/**
 * Outputs the contents of the given session object into a .txt file
 * @param optional<tradeSession::session> session
 */
static void toFile(std::optional<tradeSession::session> session) {
    using namespace boost::posix_time;
    using namespace boost::local_time;
    using namespace std;
    ptime time = second_clock::local_time();
    string path = "/Users/yuyang/Documents/meanreversion trades";
    string file = to_iso_string(time) + ".txt";
    ofstream out(path + "/" + file);
    out << "Session Concluded. Bought at: " << session.value().buy.currprice << endl;
    out << "                   Sold at: " << session.value().sell->currprice << endl;
    out << "                   Earnings: " << session.value().earnings << endl;
    out.close();
}

int main()
{
    const std::string directoryPath = "/Users/yuyang/Library/Application Support/net.metaquotes.wine.metatrader5/drive_c/Program Files/MetaTrader 5/MQL5/Files";
    std::vector<tradeSession::session> prev;
    std::optional<tradeSession::session> session;
    while(true) {
        for(const auto& entry : fs::directory_iterator(directoryPath)) {
            std::string file = entry.path().string();
            if (entry.path().filename() == ".DS_Store") {
                continue; // Skip this file
            }
            dataCalculations::trade temp(file, 0.4);
            if(!session.has_value()) {
                session.emplace(temp);
            }else {
                session.value().update(temp);
            }
            std::cout << session.value().tradetype << std::endl;
            if(session.value().sell.has_value()) {
                std::cout << "Trading Session Complete. Total Earnings: " << session.value().earnings << std::endl;
                toFile(session);
                prev.push_back(session.value());
                session.reset();
            }
            fs::remove(entry);
            std::this_thread::sleep_for(std::chrono::minutes(1));
        }
    }
}
