#include "strategy.hpp"

#pragma once
class MACD : public Strategy
{
    int n, x;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics macd(std::vector<double> close_price, std::vector<std::string> date, int x);

    Statistics get_stats(std::vector<std::vector<double>> data, std::vector<std::string> date);

  public:
    MACD(std::vector<std::string> args);
    ~MACD();
};