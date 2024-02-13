#include "strategy.hpp"
#pragma once

class StrategyChooser
{
    StrategyType choice;
    Strategy strategy;

  public:
    StrategyChooser(std::vector<std::string> args);
    StrategyType get_strategy_used();
    double get_final_pnl();
    std::vector<std::vector<std::string>> get_orders();

    std::vector<std::vector<std::string>> get_orders2();
    std::vector<std::vector<std::string>> get_cashflow();
};
