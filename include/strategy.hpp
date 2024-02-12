#include <string>
#include <vector>
#pragma once

enum StrategyType
{
    BasicStrat,
    DMAStrat,
    DMAPlusPlusStrat,
    MACDStrat,
    RSIStrat,
    ADXStrat,
    LinearRegressionStrat,
    BestOfAllStrat,
    PairsStrat,
    PairsSLStrat,
};

struct Statistics
{
    std::vector<std::pair<std::string, double>> daily_cashflow;
    std::vector<std::vector<std::string>> order_statistics;
    std::vector<std::vector<std::string>> order_statistics2;
    double final_pnl;
};
class Strategy
{
  public:
    Statistics stats;
    double get_final_pnl()
    {
        return stats.final_pnl;
    }
    std::vector<std::pair<std::string, double>> get_daily_cashflow()
    {
        return stats.daily_cashflow;
    };
    std::vector<std::vector<std::string>> get_order_statistics()
    {
        return stats.order_statistics;
    };
    std::vector<std::vector<std::string>> get_order_statistics2()
    {
        return stats.order_statistics2;
    };
};