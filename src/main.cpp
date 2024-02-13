#include "arg_parser.hpp"
#include "csv_writer.hpp"
#include "decider.hpp"
#include "strategy.hpp"
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    ArgParser arg(argc, argv);
    vector<string> args = arg.get_arguments();
    StrategyChooser choose(args);
    if (choose.get_strategy_used() == PairsStrat || choose.get_strategy_used() == PairsSLStrat)
    {
        CSVWriter order_statistics("order_statistics_1.csv");
        CSVWriter order_statistics2("order_statistics_2.csv");
        order_statistics.write(choose.get_orders());
        order_statistics2.write(choose.get_orders2());
    }
    else
    {
        CSVWriter order_statistics("order_statistics.csv");
        order_statistics.write(choose.get_orders());
    }
    CSVWriter daily_cashflow("daily_cashflow.csv");
    daily_cashflow.write(choose.get_cashflow());
    ofstream pnl;
    pnl.open("final_pnl.txt");
    pnl << to_string(choose.get_final_pnl());
    return 0;
}