#include "strategy.hpp"
#include <cmath>
#include <iostream>

#pragma once
class DMAPlusPlus : public Strategy
{
    int n, x, p, max_hold_days;
    double c1, c2;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics get_stats(std::vector<std::vector<double>> data, std::vector<std::string> dates);

  public:
    DMAPlusPlus(std::vector<std::string> args);
    ~DMAPlusPlus();
};