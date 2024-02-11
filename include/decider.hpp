#include "linear_regression.hpp"
#include "my_utils.hpp"
#include "strategy.hpp"
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>
#pragma once
using namespace std;

class StrategyChooser
{
    StrategyType choice;
    Strategy strategy;

  public:
    StrategyChooser(vector<string> args)
    {
        if (args.size() < 1)
        {
            throw runtime_error("argument list empty");
        }
        string input_strategy = args[0];
        args = slice(args, 1);
        if (input_strategy == "BASIC")
        {
            choice = BasicStrat;
            strategy = Basic{args};
        }
        else if (input_strategy == "DMA")
        {
            choice = DMAStrat;
            strategy = DMA{args};
        }
        else if (input_strategy == "DMA++")
        {
            choice = DMAPlusPlusStrat;
            strategy = DMAPlusPlus{args};
        }
        else if (input_strategy == "MACD")
        {
            choice = MACDStrat;
            strategy = MACD{args};
        }
        else if (input_strategy == "RSI")
        {

            choice = RSIStrat;
            strategy = RSI{args};
        }
        else if (input_strategy == "ADX")
        {
            choice = ADXStrat;
            strategy = ADX{args};
        }
        else if (input_strategy == "LINEAR_REGRESSION")
        {
            choice = LinearRegressionStrat;
            strategy = LinearRegression{args};
        }
        else if (input_strategy == "BEST_OF_ALL")
        {
            choice = BestOfAllStrat;
            strategy = BestOfAll{args};
        }
        else if (input_strategy == "PAIRS")
        {
            choice = PairsStrat;
            strategy = PairsSL{args};
        }
        else
        {
            throw runtime_error("Invalid strategy name");
        }
    }
    StrategyType get_strategy_used()
    {
        return choice;
    }
    double final_pnl()
    {
        return strategy.get_final_pnl();
    }
    std::vector<std::vector<string>> get_orders()
    {
        std::vector<std::pair<std::string, double>> typed_stats = strategy.get_order_statistics();
        std::vector<std::vector<string>> orders(typed_stats.size(), std::vector<std::string>(2, ""));
        for (int i = 0; i < orders.size(); i++)
        {
            orders[i][0] = typed_stats[i].first;
            orders[i][1] = to_string(typed_stats[i].second);
        }
        return orders;
    }
    std::vector<std::vector<std::string>> get_cashflow()
    {
        std::vector<std::pair<std::string, double>> typed_stats = strategy.get_order_statistics();
        std::vector<std::vector<string>> cashflow(typed_stats.size(), std::vector<std::string>(2, ""));
        for (int i = 0; i < cashflow.size(); i++)
        {
            cashflow[i][0] = typed_stats[i].first;
            cashflow[i][1] = to_string(typed_stats[i].second);
        }
        return cashflow;
    }
};
