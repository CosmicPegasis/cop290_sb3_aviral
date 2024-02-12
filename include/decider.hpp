#include "adx.hpp"
#include "basic.hpp"
#include "best_of_all.hpp"
#include "dma++.hpp"
#include "dma.hpp"
#include "linear_regression.hpp"
#include "macd.hpp"
#include "my_utils.hpp"
#include "pairs.hpp"
#include "pairs_sl.hpp"
#include "rsi.hpp"
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
            if (args.size() == 7)
            {
                choice = PairsStrat;
                strategy = Pairs{args};
            }
            else if (args.size() == 8)
            {
                choice = PairsSLStrat;
                strategy = PairsSL{args};
            }
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
    double get_final_pnl()
    {
        return strategy.get_final_pnl();
    }
    std::vector<std::vector<string>> get_orders()
    {
        return strategy.get_order_statistics();
    }
    std::vector<std::vector<std::string>> get_orders2()
    {
        return strategy.get_order_statistics2();
    }
    std::vector<std::vector<std::string>> get_cashflow()
    {
        std::vector<std::pair<std::string, double>> typed_stats = strategy.get_daily_cashflow();
        std::vector<std::vector<string>> cashflow(typed_stats.size(), std::vector<std::string>(2, ""));
        for (int i = 0; i < cashflow.size(); i++)
        {
            cashflow[i][0] = typed_stats[i].first;
            cashflow[i][1] = to_string(typed_stats[i].second);
        }
        return cashflow;
    }
};
