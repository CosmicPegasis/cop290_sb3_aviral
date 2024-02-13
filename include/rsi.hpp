#include "strategy.hpp"
#include <string>
#include <vector>

#pragma once
class RSI : public Strategy
{
    int n, x;
    double oversold, overbought;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics get_stats(std::vector<std::vector<double>> data, std::vector<std::string> date);

  public:
    RSI(std::vector<std::string> args);
    ~RSI();
};