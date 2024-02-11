#include "csv_parser.hpp"
#include <iostream>
#include <stdexcept>
#include <strategy.hpp>
#pragma once
using namespace std;
class Basic : public Strategy
{
    int n, x;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args)
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

    void get_data()
    {
        std::string command =
            "python3 api.py BASIC " + symbol + " " + std::to_string(n) + " " + start + " " + end + " basic.csv";
        system(command.c_str());
        ClosePriceParser csv("basic.csv");
        this->dates = csv.get_dates();
        this->close_prices = csv.get_close_price();
    }

    Statistics get_stats(vector<vector<double>> data, vector<string> date)
    {
        vector<pair<string, double>> cashflow;
        vector<vector<string>> order_stats;
        int idx;
        int collecter = 0;
        int position = 0;
        double cash_transaction = 0;
        for (idx = 1; idx <= n; idx++)
        {
            if (data[idx][0] >= data[idx - 1][0])
            {
                collecter++;
                data[idx][1] = 1;
            }
            else
            {
                collecter--;
                data[idx][1] = -1;
            }
        }

        int lower = 0;
        int upper = idx - 1;
        while (upper < data.size())
        {
            if (collecter == n)
            {
                if (position < x)
                {
                    cash_transaction = cash_transaction - data[upper][0];
                    position++;
                    // cout << position << endl;
                    // cashflow.push_back(make_pair(date[upper], cash_transaction));
                    vector<string> v = {date[upper], "BUY", "1", to_string(data[upper][0])};
                    order_stats.push_back(v);
                }
            }
            else if (collecter == -n)
            {
                if (position > -x)
                {
                    cash_transaction = cash_transaction + data[upper][0];
                    position--;
                    // cout << position << endl;
                    // cashflow.push_back(make_pair(date[upper], cash_transaction));
                    vector<string> v = {date[upper], "SELL", "1", to_string(data[upper][0])};
                    order_stats.push_back(v);
                }
            }
            if (upper == data.size() - 1)
            {
                ;
            }
            else
            {
                if (data[upper + 1][0] >= data[upper][0])
                {

                    data[upper + 1][1] = 1;
                }
                else
                {

                    data[upper + 1][1] = -1;
                }

                collecter = collecter + data[upper + 1][1];

                collecter = collecter - data[lower + 1][1];
            }
            cashflow.push_back(make_pair(date[upper], cash_transaction));
            upper++;
            lower++;
        }
        double total_profit = 0;

        total_profit = cash_transaction + (position * data.back()[0]);

        Statistics ans;
        ans.daily_cashflow = cashflow;
        ans.final_pnl = total_profit;
        ans.order_statistics = order_stats;
        return ans;
    }

  public:
    Basic(std::vector<std::string> args)
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
    ~Basic()
    {
        remove("basic.csv");
    }
};