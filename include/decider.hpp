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

  public:
    StrategyChooser(vector<string> args)
    {
        if (args.size() < 1)
        {
            throw runtime_error("argument list empty");
        }
        string input_strategy = args[0];
        args = slice(args, 1);
        Strategy strategy;
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
};
