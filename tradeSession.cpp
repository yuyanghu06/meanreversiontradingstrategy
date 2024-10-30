//
// Created by Yuyang Hu on 10/25/24.
//
#include "tradeSession.h"
namespace tradeSession {
    /**
     * Initalizes a new session object
     * For now, volume bought is assumed to be 1000, but can/will be changed in the future
     * @param dataCalculations::trade buy
     * @param dataCalculations::trade sell
     */
    session::session(dataCalculations::trade buy) : buy(buy) {
        activeTrade = false;
        tradetype = "";
        getDecision(this->buy);
    }
    /**
     * Calculates what the trader should do on a given day
     * If no trade is currently being made, the program checks and sees if the stock should currently be
     * longed (if the price of the stock rises above the predicted price + the tolernace factor)
     * shorted (if the price of the stock rises below the predicted price - the tolerance factor)
     * or held (neither of the other two)
     *
     * If a trade is currently being made, the program checks and sees
     * if longed
     * if the stock should be sold (current price dips below regression price - tolerance)
     * otherwise it holds
     * if shorted
     * the stock should be covered (current price dips above regression price - tolerance)
     * otherwise it holds
     *
     * @return decision (buy, short, hold)
     */
    std::string session::getDecision(dataCalculations::trade curr) {
        double deviation = curr.deviation;
        double risk = curr.risk;
        double currprice = curr.currprice;
        double regressionprice = curr.regressionprice;
        double tolerance = deviation * risk;
        if(!activeTrade) {
            if(currprice >= regressionprice + tolerance) {
                activeTrade = true;
                tradetype = "long";
                return std::string("buy");
            }
            if(currprice < regressionprice - tolerance) {
                activeTrade = true;
                tradetype = "short";
                return std::string("short");
            }
        }
        if(tradetype == "long"){
            if(currprice <= regressionprice - tolerance) {
                activeTrade = false;
                tradetype = "";
                return std::string("sell");
            }
        }
        if(tradetype == "short") {
            if(currprice >= regressionprice - tolerance) {
                activeTrade = false;
                tradetype = "";
                return std::string("cover");
            }
         }
        return std::string("hold");
    }

    /**
     * Updates the session with current data
     * Ends the trade session if the system decides to sell, or if earnings go negative
     * @param curr
     */
    void session::update(dataCalculations::trade curr) {
        if(!activeTrade) {
            getDecision(curr);
            if(tradetype == "hold") {
                return;
            }
            buy = curr;
        }else {
            std::string decision = getDecision(curr);
            if(decision == "sell") {
                sell = curr;
                earnings = getNetEarningsLong(buy.currprice, curr.currprice);
            }
            if(decision == "cover") {
                sell = curr;
                earnings = getNetEarningsShort(buy.currprice, curr.currprice);
            }
            if(decision == "hold") {
                if(tradetype == "short") {
                    if(getNetEarningsShort(buy.currprice, curr.currprice) <= 0) {
                        activeTrade = false;
                        sell = curr;
                        tradetype = "";
                        earnings = getNetEarningsShort(buy.currprice, curr.currprice);
                    }
                }
                if(tradetype == "long") {
                    if(getNetEarningsLong(buy.currprice, curr.currprice) <= 0) {
                        activeTrade = false;
                        sell = curr;
                        tradetype = "";
                        earnings = getNetEarningsLong(buy.currprice, curr.currprice);
                    }
                }
            }
        }
    }
}