#include "dma.hpp"
#include "csv_parser.hpp"
#include "strategy.hpp"
#include <cmath>
#include <iostream>

using namespace std;
void DMA::parse_args(std::vector<std::string> &args)
{
    if (args.size() < 6)
    {
        throw std::runtime_error("Illformed args");
    }
    symbol = args[0];
    n = stoi(args[1]);
    x = stoi(args[2]);
    p = stoi(args[3]);
    start = args[4];
    end = args[5];
    std::cout << "symbol " << symbol;
    std::cout << "n " << n;
    std::cout << "x " << x;
    std::cout << "p " << p;
    std::cout << "start " << start;
    std::cout << "end " << end;
}

void DMA::get_data()
{
    std::string command =
        "python3 api.py DMA " + symbol + " " + std::to_string(n) + " " + start + " " + end + " dma.csv";
    system(command.c_str());
    ClosePriceParser csv("dma.csv");
    this->dates = csv.get_dates();
    this->close_prices = csv.get_close_price();
}

Statistics DMA::get_stats(std::vector<std::vector<double>> data, std::vector<std::string> date)
{

    vector<pair<string, double>> cashflow;
    vector<vector<string>> order_stats;
    double total_profit = 0;
    double sum = 0;
    double sum_sq = 0;
    int idx;
    int position = 0;
    double cash_transaction = 0;

    for (idx = 1; idx <= n; idx++)
    {

        sum = sum + data[idx][0];
        sum_sq = sum_sq + (data[idx][0] * data[idx][0]);
    }

    int upper = idx - 1;
    int lower = 1;

    while (upper < data.size())
    {
        double mean = sum / double(n);
        double sd = sqrt(((sum_sq) / double(n)) - (mean * mean));
        // cout << date[upper] << " " << data[upper][0] - mean << "->" << mean << " " << sd << " " << p * sd << " "
        // << data[upper][0] << endl;

        if (data[upper][0] >= mean + (p * sd))
        {

            if (position < x)
            {
                cash_transaction = cash_transaction - data[upper][0];
                position++;
                // cout << position << endl;

                vector<string> v = {date[upper], "BUY", "1", to_string(data[upper][0])};
                order_stats.push_back(v);
            }
        }
        else if (mean >= data[upper][0] + (p * sd))
        {
            if (position > -x)
            {
                cash_transaction = cash_transaction + data[upper][0];
                position--;
                // cout << position << endl;

                vector<string> v = {date[upper], "SELL", "1", to_string(data[upper][0])};
                order_stats.push_back(v);
            }
        }
        cashflow.push_back(make_pair(date[upper], cash_transaction));
        if (upper != data.size() - 1)
        {
            sum = sum + data[upper + 1][0];

            sum_sq = sum_sq + (data[upper + 1][0] * data[upper + 1][0]);

            sum = sum - data[lower][0];

            sum_sq = sum_sq - (data[lower][0] * data[lower][0]);
        }

        lower++;
        upper++;
        // cout << "----------" << endl;
    }

    total_profit = cash_transaction + (position * data.back()[0]);
    // cout << total_profit << endl;

    Statistics ans;
    ans.daily_cashflow = cashflow;
    ans.final_pnl = total_profit;
    ans.order_statistics = order_stats;

    // cout << total_profit << endl;
    return ans;
}

DMA::DMA(std::vector<std::string> args)
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
DMA::~DMA()
{
    remove("dma.csv");
}