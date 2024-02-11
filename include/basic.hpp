#include "csv_parser.hpp"
#include <iostream>
#include <stdexcept>
#include <strategy.hpp>
using namespace std;

class Basic : public Strategy
{
    int n, x;
    string symbol, start, end;
    vector<string> dates;
    vector<double> close_prices;
    void parse_args(vector<string> &args)
    {
        if (args.size() < 5)
        {
            throw runtime_error("Illformed args");
        }
        symbol = args[0];
        n = stoi(args[1]);
        x = stoi(args[2]);
        start = args[3];
        end = args[4];
    }

    void get_data()
    {
        string command = "python3 api.py BASIC " + symbol + " " + to_string(n) + " " + start + " " + end + " basic.csv";
        system(command.c_str());
        ClosePriceParser csv("basic.csv");
        this->dates = csv.get_dates();
        this->close_prices = csv.get_close_price();
    }
    Statistics get_stats(vector<vector<double>> close_price, vector<string> dates)
    {
        vector<pair<string, double>> cashflow;
        vector<vector<string>> order_stats;
        int idx;
        int collecter = 0;
        int position = 0;
        double cash_transaction = 0;
        for (idx = 1; idx < n; idx++)
        {
            if (close_price[idx][0] >= close_price[idx - 1][0])
            {
                collecter++;
                close_price[idx][1] = 1;
            }
            else
            {
                collecter--;
                close_price[idx][1] = -1;
            }
        }

        int lower = 0;
        int upper = idx;
        while (upper < close_price.size())
        {
            if (collecter == n - 1)
            {
                if (position < x)
                {
                    cash_transaction = cash_transaction - close_price[upper][0];
                    position++;
                    // cout << position << endl;
                    cashflow.push_back(make_pair(dates[upper], cash_transaction));
                    vector<string> v = {dates[upper], "BUY", "1", to_string(close_price[upper][0])};
                    order_stats.push_back(v);
                }
                if (close_price[upper][0] >= close_price[upper - 1][0])
                {

                    close_price[upper][1] = 1;
                }
                else
                {

                    close_price[upper][1] = -1;
                }

                collecter = collecter + close_price[upper][1];

                collecter = collecter - close_price[lower + 1][1];

                upper++;
                lower++;
            }
            else if (collecter == -n + 1)
            {
                if (position > -x)
                {
                    cash_transaction = cash_transaction + close_price[upper][0];
                    position--;
                    // cout << position << endl;
                    cashflow.push_back(make_pair(dates[upper], cash_transaction));
                    vector<string> v = {dates[upper], "SELL", "1", to_string(close_price[upper][0])};
                    order_stats.push_back(v);
                }
                if (close_price[upper][0] >= close_price[upper - 1][0])
                {

                    close_price[upper][1] = 1;
                }
                else
                {

                    close_price[upper][1] = -1;
                }

                collecter = collecter + close_price[upper][1];

                collecter = collecter - close_price[lower + 1][1];

                upper++;
                lower++;
            }
            else
            {

                if (close_price[upper][0] >= close_price[upper - 1][0])
                {

                    close_price[upper][1] = 1;
                }
                else
                {

                    close_price[upper][1] = -1;
                }

                collecter = collecter + close_price[upper][1];

                collecter = collecter - close_price[lower + 1][1];

                upper++;
                lower++;
            }
        }
        double total_profit = 0;

        total_profit = cash_transaction + (position * close_price.back()[0]);
        // cout << total_profit << endl;

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
        vector<vector<double>> transformed_close_prices{close_prices.size()};
        // Make a 2D vector instead of a 1D vector
        for (int i = 0; i < close_prices.size(); i++)
        {
            transformed_close_prices[i].push_back(close_prices[i]);
            transformed_close_prices[i].push_back(1);
        }
        this->stats = get_stats(transformed_close_prices, dates);
    };
};