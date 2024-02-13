#include <iostream>
#include <strategy.hpp>
#include <vector>

#pragma once
class ADX : public Strategy
{
    int n, x;
    double adx_threshold;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<std::vector<double>> data;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics get_stats(std::vector<std::vector<double>> data, std::vector<std::string> dates);

  public:
    ADX(std::vector<std::string> args);
    ~ADX();
};