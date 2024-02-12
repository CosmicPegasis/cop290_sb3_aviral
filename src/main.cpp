// #include "linear_regression.cpp"
#include "arg_parser.hpp"
#include "csv_writer.hpp"
#include "decider.hpp"
#include "strategy.hpp"
#include <string>
#include <vector>

// TODO Check floating points by checking what values are ints
// TODO RSI daily cashflow is wrong.
using namespace std;

int main(int argc, char **argv)
{
    ArgParser arg(argc, argv);
    vector<string> args = arg.get_arguments();
    StrategyChooser choose(args);
    CSVWriter order_statistics("order_statistics.csv");
    CSVWriter daily_cashflow("daily_cashflow.csv");
    if (choose.get_strategy_used() == PairsStrat || choose.get_strategy_used() == PairsSLStrat)
    {
        CSVWriter order_statistics2("order_statistics2.csv");
        order_statistics2.write(choose.get_orders2());
    }
    order_statistics.write(choose.get_orders());
    daily_cashflow.write(choose.get_cashflow());
    ofstream pnl;
    pnl.open("final_pnl.txt");
    pnl << to_string(choose.get_final_pnl());
    return 0;
}

// Strategy Decider
/*
void basic(std::vector<std::vector<double>> data, int n, int x, std::vector<std::string> date);

void dma(vector<vector<double>> data, int n, int x, vector<string> date, int p);

void dma(vector<vector<double>> data, int n, int x, vector<string> date, double p,int max_hold_days,double c1,double c2)

void macd(vector<double> close_price, vector<double> open_price, vector<string> date, int x)

void rsi(vector<vector<double>> data, int n, int x, vector<string> date, double oversold, double overbought)


void BestOfALL // TODO

void pairs_sl(vector<vector<double>> data, int n, int x, vector<string> date, int p, double threshold,double
stop_loss_threshold)
 */
