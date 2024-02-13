#include "strategy.hpp"
#include <iostream>
#include <stdexcept>
#pragma once
class Basic : public Strategy
{
    int n, x;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics get_stats(std::vector<std::vector<double>> data, std::vector<std::string> date);

  public:
    Basic(std::vector<std::string> args);
    ~Basic();
};