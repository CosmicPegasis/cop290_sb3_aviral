#include "pairs.hpp"
#include "csv_parser.hpp"
#include <cmath>
#include <iostream>
#include <strategy.hpp>

using namespace std;
void Pairs::parse_args(std::vector<std::string> &args)
{
    if (args.size() < 7)
    {
        throw std::runtime_error("Illformed args");
    }
    symbol1 = args[0];
    symbol2 = args[1];
    x = stoi(args[2]);
    n = stoi(args[3]);
    threshold = stod(args[4]);
    start = args[5];
    end = args[6];
}

void Pairs::get_data()
{
    std::string command1 = "python3 api.py BASIC " + symbol1 + " " + std::to_string(n) + " " + start + " " + end +
                           " pairs_" + symbol1 + ".csv";
    std::string command2 = "python3 api.py BASIC " + symbol2 + " " + std::to_string(n) + " " + start + " " + end +
                           " pairs_" + symbol2 + ".csv";
    system(command1.c_str());
    system(command2.c_str());
    ClosePriceParser csv1(("pairs_" + symbol1 + ".csv").c_str());
    ClosePriceParser csv2(("pairs_" + symbol2 + ".csv").c_str());
    this->dates = csv1.get_dates();
    vector<double> price1 = csv1.get_close_price();
    vector<double> price2 = csv2.get_close_price();
    data = vector<vector<double>>(price1.size(), vector<double>(2));
    for (int i = 0; i < price1.size(); i++)
    {
        data[i][0] = price1[i];
        data[i][1] = price2[i];
    }
}

Statistics Pairs::get_stats()
{
    vector<pair<string, double>> cashflow;
    vector<vector<string>> order_stats;
    vector<vector<string>> order_stats1;

    double total_profit = 0;

    int position = 0;
    double cash_transaction = 0;

    for (int idx = n; idx < data.size(); idx++)
    {
        double sum = 0;
        double sum_sq = 0;
        for (int i = idx + 1 - n; i <= idx; i++)
        {
            sum = sum + data[i][0] - data[i][1];
            sum_sq = sum_sq + ((data[i][0] - data[i][1]) * (data[i][0] - data[i][1]));
        }
        double mean = (sum / (double(n)));
        double sd = sqrt((sum_sq / double(n)) - (mean * mean));

        double z_score = ((data[idx][0] - data[idx][1]) - mean) / sd;
        if (z_score < -threshold)
        {
            if (position < x)
            {
                cash_transaction = cash_transaction - data[idx][0] + data[idx][1];
                position++;
                vector<string> v = {dates[idx], "BUY", "1", to_string(data[idx][0])};
                order_stats.push_back(v);
                vector<string> v1 = {dates[idx], "SELL", "1", to_string(data[idx][1])};
                order_stats1.push_back(v1);
            }
        }
        if (z_score > threshold)
        {
            if (position > -x)
            {
                cash_transaction = cash_transaction + data[idx][0] - data[idx][1];
                position--;
                vector<string> v = {dates[idx], "SELL", "1", to_string(data[idx][0])};
                order_stats.push_back(v);
                vector<string> v1 = {dates[idx], "BUY", "1", to_string(data[idx][1])};
                order_stats1.push_back(v1);
            }
        }

        cashflow.push_back(make_pair(dates[idx], cash_transaction));
    }

    total_profit = cash_transaction + (position * (data.back()[0] - data.back()[1]));
    Statistics ans;
    ans.daily_cashflow = cashflow;
    ans.final_pnl = total_profit;
    ans.order_statistics = order_stats;
    ans.order_statistics2 = order_stats1;
    return ans;
}

Pairs::Pairs(std::vector<std::string> args)
{
    this->parse_args(args);
    this->get_data();
    this->stats = get_stats();
};
Pairs::~Pairs()
{
    remove(("pairs_" + symbol1 + ".csv").c_str());
    remove(("pairs_" + symbol2 + ".csv").c_str());
}