#include "csv_parser.hpp"
#include "strategy.hpp"
#include <cmath>
#include <iostream>

#pragma once
using namespace std;
class MACD : public Strategy
{
    int n, x;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics macd(vector<double> close_price, vector<string> date, int x);

    Statistics get_stats(vector<vector<double>> data, vector<string> date);

  public:
    MACD(std::vector<std::string> args);
    ~MACD();
};