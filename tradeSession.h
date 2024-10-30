//
// Created by Yuyang Hu on 10/25/24.
//

#ifndef TRADESESSION_H
#define TRADESESSION_H
#include "dataCalculations.h"
#include <optional>
namespace tradeSession {
    /**
    * Returns the net earnings of a long trade
    * @param double buy
    * @param double curr
    * @return difference between the two
    */
    double static getNetEarningsLong(double buy, double curr) {
        return 1000 * (curr - buy);
    }
    /**
     * Returns the net earnings of a short trade
     * @param double buy
     * @param double curr
     * @return difference between the two
     */
    double static getNetEarningsShort(double buy, double curr) {
        return 1000 * (buy - curr);
    }
    struct session {
        //Constructor Declaraion
        session(dataCalculations::trade buy);

        //Variable Declaration
        dataCalculations::trade buy;
        std::optional<dataCalculations::trade> sell;
        std::string tradetype;
        bool activeTrade;
        double earnings;

        //function declarations
        std::string getDecision(dataCalculations::trade curr);
        void update(dataCalculations::trade curr);
    };
}
#endif //TRADESESSION_H
