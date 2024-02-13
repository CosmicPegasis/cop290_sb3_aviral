#include "best_of_all.hpp"
#include "adx.hpp"
#include "basic.hpp"
#include "dma++.hpp"
#include "dma.hpp"
#include "linear_regression.hpp"
#include "macd.hpp"
#include "rsi.hpp"
#include "strategy.hpp"
#include <cmath>
#include <iostream>
#include <omp.h>

using namespace std;
void BestOfAll::parse_args(std::vector<std::string> &args)
{
    if (args.size() < 3)
    {
        throw std::runtime_error("Illformed args");
    }
    symbol = args[0];
    start = args[1];
    end = args[2];
    cout << "symbol " << symbol << endl;
    cout << "start " << start << endl;
    cout << "end " << end << endl;
}

void BestOfAll::get_data()
{
    strategies = vector<Strategy>(7);
#pragma omp parallel
#pragma omp single
    {
#pragma omp task
        // cout << "Basic" << endl;
        strategies[0] = Basic{{symbol, basic_n, x, start, end}};

#pragma omp task
        // cout << "DMA" << endl;
        strategies[1] = DMA{{symbol, dma_n, x, dma_p, start, end}};

#pragma omp task
        // cout << "DMAPP" << endl;
        strategies[2] = DMAPlusPlus{{symbol, x, dmapp_p, dmapp_n, dmapp_max_hold_days, dmapp_c1, dmapp_c2, start, end}};

#pragma omp task
        // cout << "MACD" << endl;
        strategies[3] = MACD{{symbol, x, start, end}};

#pragma omp task
        // cout << "RSI" << endl;
        strategies[4] = RSI{{symbol, x, dmapp_n, rsi_oversold, rsi_overbought, start, end}};

#pragma omp task
        // cout << "ADX" << endl;
        strategies[5] = ADX{{symbol, x, dmapp_n, adx_threshold, start, end}};

#pragma omp task
        // cout << "LR" << endl;
        strategies[6] = LinearRegression{symbol, stoi(x), stoi(lr_p), start, end};
    }
    // strategies = vector<Strategy>{basic, dma, dmapp, macd, rsi, adx, lr};
}
Statistics BestOfAll::get_stats()
{
    double max = strategies[0].get_final_pnl();
    int max_index = 0;
    for (int i = 0; i < strategies.size(); i++)
    {
        if (max < strategies[i].get_final_pnl())
        {
            max_index = i;
            max = strategies[i].get_final_pnl();
        }
    }
    cout << max_index << endl;
    return strategies[max_index].stats;
}

BestOfAll::BestOfAll(std::vector<std::string> args)
{
    this->parse_args(args);
    this->get_data();
    this->stats = this->get_stats();
};
BestOfAll::~BestOfAll()
{
}