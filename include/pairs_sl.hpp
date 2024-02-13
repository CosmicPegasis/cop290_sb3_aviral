#include "strategy.hpp"

#pragma once
class PairsSL : public Strategy
{
    std::string symbol1, symbol2, start, end;
    int x, n;
    double threshold;
    double stop_loss_threshold;
    std::vector<std::string> dates;
    std::vector<std::vector<double>> data;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics get_stats();

  public:
    PairsSL(std::vector<std::string> args);
    ~PairsSL();
};
