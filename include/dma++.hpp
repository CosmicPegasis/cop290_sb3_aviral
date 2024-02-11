#include <basic.hpp>
#include <cmath>
#include <iostream>
#include <strategy.hpp>

#pragma once
using namespace std;
class DMAPlusPlus : public Strategy
{
    int n, x, p, max_hold_days;
    double c1, c2;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args)
    {
        if (args.size() < 9)
        {
            throw std::runtime_error("Illformed args");
        }
        symbol = args[0];
        x = stoi(args[1]);
        p = stoi(args[2]);
        n = stoi(args[3]);
        max_hold_days = stoi(args[4]);
        c1 = stod(args[5]);
        c2 = stod(args[6]);
        start = args[7];
        end = args[8];
        cout << "symbol " << symbol << endl;
        cout << "x " << x << endl;
        cout << "p " << p << endl;
        cout << "n " << n << endl;
        cout << "max_hold_days " << max_hold_days << endl;
        cout << "c1 " << c1 << endl;
        cout << "c2 " << c2 << endl;
        cout << "start " << start << endl;
        cout << "end " << end << endl;
    }

    void get_data()
    {
        std::string command =
            "python3 api.py DMA++ " + symbol + " " + std::to_string(n) + " " + start + " " + end + " dmaplusplus.csv";
        system(command.c_str());
        ClosePriceParser csv("dmaplusplus.csv");
        this->dates = csv.get_dates();
        this->close_prices = csv.get_close_price();
    }

    Statistics get_stats(vector<vector<double>> close_price, vector<string> dates)
    {
        vector<pair<string, double>> cashflow;
        vector<vector<string>> order_stats;
        double total_profit = 0;
        double cash_transaction = 0;
        int position = 0;
        vector<int> days_monitor;
        int idx = n - 1;
        double SF = 0.5;
        double AMA = close_price[idx][0];
        while (idx < close_price.size())
        {

            for (int i = 0; i < days_monitor.size(); i++)
            {
                days_monitor[i]++;
            }
            double absolute_change = 0;
            for (int i = idx - n + 2; i <= idx; i++)
            {
                if (close_price[i][0] - close_price[i - 1][0] >= 0)
                {
                    absolute_change += close_price[i][0] - close_price[i - 1][0];
                }
                else
                {
                    absolute_change += close_price[i - 1][0] - close_price[i][0];
                }
            }
            double ER = close_price[idx][0] - close_price[idx + 1 - n][0];
            bool flag = false;

            if (absolute_change == 0)
            {
                flag = true;
            }
            else
            {
                ER = ER / absolute_change;
            }

            // cout << ER << "->" << dates[idx] << endl;
            double num = double(2 * ER) / double(1 + c2);
            if (idx == n - 1)
            {
                SF = 0.5;
                AMA = close_price[idx][0];
            }
            else
            {
                if (absolute_change == 0)
                {
                    SF = SF;
                }
                else
                {
                    SF = SF + (c1 * ((double(num - 1) / double(num + 1)) - SF));
                }
                AMA = AMA + (SF * (close_price[idx][0] - AMA));
            }
            string s = "none";
            // cout << dates[idx][0] << "->" << SF << "->" << AMA << endl;
            if (days_monitor.size() > 0)
            {
                if (days_monitor[0] >= max_hold_days)
                {
                    if (position > 0)
                    {
                        s = "sell";
                        cash_transaction = cash_transaction + close_price[idx][0];
                        days_monitor.erase(days_monitor.begin() + 0);
                        position--;
                        // cout << "SELLO" << endl;
                        // cout << position << endl;
                        cashflow.push_back(make_pair(dates[idx], cash_transaction));
                        vector<string> v = {dates[idx], "SELL", "1", to_string(close_price[idx][0])};
                        order_stats.push_back(v);
                    }
                    else
                    {
                        s = "buy";
                        cash_transaction = cash_transaction - close_price[idx][0];
                        // cout << days_monitor.size() << endl;
                        days_monitor.erase(days_monitor.begin() + 0);
                        // cout << "BUYO" << endl;
                        position++;
                        // cout << position << endl;
                        cashflow.push_back(make_pair(dates[idx], cash_transaction));
                        vector<string> v = {dates[idx], "BUY", "1", to_string(close_price[idx][0])};
                        order_stats.push_back(v);
                    }
                }
            }

            if (close_price[idx][0] > AMA + ((AMA * p) / double(100)))
            {
                if (position < x)
                {
                    if (s == "none")
                    {
                        cash_transaction = cash_transaction - close_price[idx][0];
                        if (position < 0)
                        {
                            days_monitor.erase(days_monitor.begin() + 0);
                        }
                        else
                        {
                            days_monitor.push_back(0);
                        }
                        position++;
                        // cout << position << endl;
                        cashflow.push_back(make_pair(dates[idx], cash_transaction));
                        vector<string> v = {dates[idx], "BUY", "1", to_string(close_price[idx][0])};
                        order_stats.push_back(v);
                    }
                    else if (s == "sell")
                    {
                        cash_transaction = cash_transaction - close_price[idx][0];
                        if (position < 0)
                        {
                            days_monitor.erase(days_monitor.begin() + 0);
                        }
                        else
                        {
                            days_monitor.push_back(0);
                        }
                        position++;
                        // cout << position << endl;
                    }
                    else if (s == "buy")
                    {
                        ;
                    }
                }
            }
            else if (close_price[idx][0] < AMA - ((AMA * p) / double(100)))
            {
                if (position > -x)
                {
                    if (s == "none")
                    {
                        cash_transaction = cash_transaction + close_price[idx][0];
                        if (position > 0)
                        {
                            days_monitor.erase(days_monitor.begin() + 0);
                        }
                        else
                        {
                            days_monitor.push_back(0);
                        }
                        position--;
                        // cout << position << endl;
                        cashflow.push_back(make_pair(dates[idx], cash_transaction));
                        vector<string> v = {dates[idx], "SELL", "1", to_string(close_price[idx][0])};
                        order_stats.push_back(v);
                    }
                    else if (s == "buy")
                    {
                        cash_transaction = cash_transaction + close_price[idx][0];
                        if (position > 0)
                        {
                            days_monitor.erase(days_monitor.begin() + 0);
                        }
                        else
                        {
                            days_monitor.push_back(0);
                        }
                        position--;
                        // cout << position << endl;
                    }
                    else if (s == "sell")
                    {
                        ;
                    }
                }
            }

            idx++;
        }

        total_profit = cash_transaction + (position * close_price.back()[0]);
        // cout << total_profit << endl;

        Statistics ans;
        ans.daily_cashflow = cashflow;
        ans.final_pnl = total_profit;
        ans.order_statistics = order_stats;
        cout << total_profit << endl;

        return ans;
    }

  public:
    DMAPlusPlus(std::vector<std::string> args)
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
    ~DMAPlusPlus()
    {
        remove("dmaplusplus.csv");
    }
};