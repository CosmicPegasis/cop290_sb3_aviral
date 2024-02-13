#include "basic.hpp"
#include "csv_parser.hpp"
#include <iostream>
#include <stdexcept>
#include <strategy.hpp>

using namespace std;
void Basic::parse_args(std::vector<std::string> &args)
{
    if (args.size() < 5)
    {
        throw std::runtime_error("Illformed args");
    }
    symbol = args[0];
    n = stoi(args[1]);
    x = stoi(args[2]);
    start = args[3];
    end = args[4];
}

void Basic::get_data()
{
    std::string command =
        "python3 api.py BASIC " + symbol + " " + std::to_string(n) + " " + start + " " + end + " basic.csv";
    system(command.c_str());
    ClosePriceParser csv("basic.csv");
    this->dates = csv.get_dates();
    this->close_prices = csv.get_close_price();
}

Statistics Basic::get_stats(vector<vector<double>> data, vector<string> date)
{
    vector<pair<string, double>> cashflow;
    vector<vector<string>> order_stats;
    int idx;
    int collecter = 0;
    int position = 0;
    double cash_transaction = 0;
    // cout<<"Function call"<<endl;

    for (int idx = n; idx < data.size(); idx++)
    {
        bool inc = true;
        bool dec = true;
        for (int i = idx - n + 1; i <= idx; i++)
        {
            if (data[i][0] > data[i - 1][0])
            {
                dec = false;
            }
            if (data[i][0] < data[i - 1][0])
            {
                inc = false;
            }
            if (data[i][0] == data[i - 1][0])
            {
                inc = false;
                dec = false;
            }
        }

        if (inc)
        {
            if (position < x)
            {
                cash_transaction = cash_transaction - data[idx][0];
                position++;
                vector<string> v = {date[idx], "BUY", "1", to_string(data[idx][0])};
                order_stats.push_back(v);
            }
        }
        if (dec)
        {
            if (position > -x)
            {
                cash_transaction = cash_transaction + data[idx][0];
                position--;
                vector<string> v = {date[idx], "SELL", "1", to_string(data[idx][0])};
                order_stats.push_back(v);
            }
        }
        // cout<<date[idx]<<"->"<<cash_transaction<<endl;
        cashflow.push_back(make_pair(date[idx], cash_transaction));
    }

    double total_profit = 0;

    total_profit = cash_transaction + (position * data.back()[0]);
    // cout << total_profit << endl;

    Statistics ans;
    ans.daily_cashflow = cashflow;
    ans.final_pnl = total_profit;
    ans.order_statistics = order_stats;
    return ans;
}

Basic::Basic(std::vector<std::string> args)
{
    this->parse_args(args);
    this->get_data();
    std::vector<std::vector<double>> transformed_close_prices{close_prices.size()};
    // Make a 2D vector instead of a 1D vector
    for (int i = 0; i < close_prices.size(); i++)
    {
        transformed_close_prices[i].push_back(close_prices[i]);
        transformed_close_prices[i].push_back(1);
    }
    this->stats = get_stats(transformed_close_prices, dates);
};
Basic::~Basic()
{
    remove("basic.csv");
}