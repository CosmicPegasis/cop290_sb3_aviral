#include "strategy.hpp"
#include <iostream>

#pragma once
using namespace std;
class PairsSL : public Strategy
{
    string symbol1, symbol2, start, end;
    int x, n;
    double threshold;
    double stop_loss_threshold;
    vector<string> dates;
    vector<vector<double>> data;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics get_stats();

  public:
    PairsSL(std::vector<std::string> args);
    ~PairsSL();
};
