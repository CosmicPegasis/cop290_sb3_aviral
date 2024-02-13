#include "strategy.hpp"
#include <cmath>
#include <iostream>

#pragma once
using namespace std;
class DMA : public Strategy
{
    int n, x, p;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics get_stats(std::vector<std::vector<double>> data, std::vector<std::string> date);

  public:
    DMA(std::vector<std::string> args);
    ~DMA();
};