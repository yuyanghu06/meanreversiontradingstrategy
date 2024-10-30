#include "dataCalculations.h"
#include "tradeSession.h"
#include <chrono>
#include <thread>
#include <iostream>

namespace fs = std::filesystem;
int main()
{
    const std::string directoryPath = "/Users/yuyang/Library/Application Support/net.metaquotes.wine.metatrader5/drive_c/Program Files/MetaTrader 5/MQL5/Files";
    std::vector<tradeSession::session> prev;
    while(true) {
        std::optional<tradeSession::session> session;
        for(const auto& entry : fs::directory_iterator(directoryPath)) {
            std::string file = entry.path().string();
            if (entry.path().filename() == ".DS_Store") {
                continue; // Skip this file
            }
            dataCalculations::trade temp(file, 0.2);
            if(!session.has_value()) {
                session.emplace(temp);
            }else {
                session.value().update(temp);
            }
            std::cout << session.value().tradetype << std::endl;
            if(session.value().sell.has_value()) {
                std::cout << "Trading session has concluded, total earnings: " << session.value().earnings << std::endl;
                prev.push_back(session.value());
                session.reset();
            }
            fs::remove(entry);
            std::this_thread::sleep_for(std::chrono::minutes(1));
        }
    }

    std::string filename = "/Users/yuyang/Documents/CLion Projects/meanreversiontradingstrategy/10102024225213.csv";
    dataCalculations::trade t(filename, 0.2);
    std::cout << t.deviation << std::endl;
    std::cout << t.regressionprice << std::endl;
    std::cout << t.decision << std::endl;
}
