#include "macd.hpp"
#include "csv_parser.hpp"
#include "strategy.hpp"
#include <cmath>
#include <iostream>

using namespace std;
void MACD::parse_args(std::vector<std::string> &args)
{
    if (args.size() < 4)
    {
        throw std::runtime_error("Illformed args");
    }
    symbol = args[0];
    x = stoi(args[1]);
    start = args[2];
    end = args[3];
}

void MACD::get_data()
{
    std::string command = "python3 api.py MACD " + symbol + " " + "34" + " " + start + " " + end + " macd.csv";
    system(command.c_str());
    ClosePriceParser csv("macd.csv");
    this->dates = csv.get_dates();
    this->close_prices = csv.get_close_price();
}

Statistics MACD::macd(vector<double> close_price, vector<string> date, int x)
{

    vector<pair<string, double>> cashflow;
    vector<vector<string>> order_stats;
    int position = 0;
    int idx;
    double cash_transaction = 0;
    double MACD = 0;
    double short_ewm = 0;
    double long_ewm = 0;
    double short_ewm_alpha = double(2) / double(12 + 1);
    double long_ewm_alpha = double(2) / double(26 + 1);
    double signal_line_alpha = double(2) / double(9 + 1);
    double signal = 0;

    for (idx = 34; idx < close_price.size(); idx++)
    {
        if (idx == 34)
        {
            short_ewm = close_price[idx];
            long_ewm = close_price[idx];
            MACD = short_ewm - long_ewm;
            signal = MACD;
        }
        else
        {
            short_ewm = short_ewm + (short_ewm_alpha * (close_price[idx] - short_ewm));
            long_ewm = long_ewm + (long_ewm_alpha * (close_price[idx] - long_ewm));
            MACD = short_ewm - long_ewm;
            signal = signal + (signal_line_alpha * (MACD - signal));
        }

        if (MACD > signal)
        {
            if (position < x)
            {
                cash_transaction = cash_transaction - close_price[idx];
                position++;

                vector<string> v = {date[idx], "BUY", "1", to_string(close_price[idx])};
                order_stats.push_back(v);
            }
        }
        if (MACD < signal)
        {
            if (position > -x)
            {
                cash_transaction = cash_transaction + close_price[idx];
                position--;

                vector<string> v = {date[idx], "SELL", "1", to_string(close_price[idx])};
                order_stats.push_back(v);
            }
        }

        cashflow.push_back(make_pair(date[idx], cash_transaction));
    }
    double total_profit = 0;
    total_profit = cash_transaction + (position * close_price.back());

    Statistics ans;
    ans.daily_cashflow = cashflow;
    ans.final_pnl = total_profit;
    ans.order_statistics = order_stats;

    return ans;
}

Statistics MACD::get_stats(vector<vector<double>> data, vector<string> date)
{
    vector<double> close_price;
    for (int i = 0; i < data.size(); i++)
    {
        close_price.push_back(data[i][0]);
    }
    return macd(close_price, date, x);
}

MACD::MACD(std::vector<std::string> args)
{
    this->parse_args(args);
    this->get_data();
    std::vector<std::vector<double>> transformed_close_prices{close_prices.size()};
    // Make a 2D std::vector instead of a 1D std::vector
    for (int i = 0; i < close_prices.size(); i++)
    {
        transformed_close_prices[i].push_back(close_prices[i]);
        transformed_close_prices[i].push_back(1);
    }
    this->stats = get_stats(transformed_close_prices, dates);
};
MACD::~MACD()
{
    remove("macd.csv");
}