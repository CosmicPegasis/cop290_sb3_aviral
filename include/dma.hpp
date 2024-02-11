#include <basic.hpp>
#include <cmath>
#include <iostream>

#pragma once
class DMA : public Strategy
{
    int n, x, p;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args)
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

    void get_data()
    {
        std::string command =
            "python3 api.py DMA " + symbol + " " + std::to_string(n) + " " + start + " " + end + " dma.csv";
        system(command.c_str());
        ClosePriceParser csv("dma.csv");
        this->dates = csv.get_dates();
        this->close_prices = csv.get_close_price();
    }

    Statistics get_stats(std::vector<std::vector<double>> close_price, std::vector<std::string> dates)
    {

        std::vector<std::pair<std::string, double>> cashflow;
        std::vector<std::vector<std::string>> order_stats;
        double total_profit = 0;
        double sum = 0;
        double sum_sq = 0;
        int idx;
        int position = 0;
        double cash_transaction = 0;

        for (idx = 0; idx < n; idx++)
        {

            sum = sum + close_price[idx][0];
            sum_sq = sum_sq + (close_price[idx][0] * close_price[idx][0]);
        }

        int upper = idx;
        int lower = 0;

        while (upper < close_price.size())
        {
            double mean = sum / double(n);
            double sd = sqrt(((sum_sq) / double(n)) - (mean * mean));
            // cout << date[upper] << " " << data[upper][0] - mean << "->" << mean << " " << sd << " " << p * sd << " "
            // << data[upper][0] << endl;

            if (close_price[upper][0] >= mean + (p * sd))
            {

                if (position < x)
                {
                    cash_transaction = cash_transaction - close_price[upper][0];
                    position++;
                    // cout << position << endl;
                    cashflow.push_back(std::make_pair(dates[upper], cash_transaction));
                    std::vector<std::string> v = {dates[upper], "BUY", "1", std::to_string(close_price[upper][0])};
                    order_stats.push_back(v);
                }
            }
            else if (mean >= close_price[upper][0] + (p * sd))
            {
                if (position > -x)
                {
                    cash_transaction = cash_transaction + close_price[upper][0];
                    position--;
                    // cout << position << endl;
                    cashflow.push_back(std::make_pair(dates[upper], cash_transaction));
                    std::vector<std::string> v = {dates[upper], "SELL", "1", std::to_string(close_price[upper][0])};
                    order_stats.push_back(v);
                }
            }

            sum = sum + close_price[upper][0];

            sum_sq = sum_sq + (close_price[upper][0] * close_price[upper][0]);

            sum = sum - close_price[lower][0];

            sum_sq = sum_sq - (close_price[lower][0] * close_price[lower][0]);

            lower++;
            upper++;
            // cout << "----------" << endl;
        }

        // double total_profit = 0;

        total_profit = cash_transaction + (position * close_price.back()[0]);
        // cout << total_profit << endl;

        Statistics ans;
        ans.daily_cashflow = cashflow;
        ans.final_pnl = total_profit;
        ans.order_statistics = order_stats;

        // cout << total_profit << endl;
        return ans;
    }

  public:
    DMA(std::vector<std::string> args)
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
    ~DMA()
    {
        remove("dma.csv");
    }
};