#include "csv_parser.hpp"
#include <cmath>
#include <iostream>
#include <strategy.hpp>

#pragma once
using namespace std;
class PairsSL : public Strategy
{
    string symbol1, symbol2, start, end;
    int x, n;
    double threshold;
    double stop_loss_threshold;
    vector<string> dates;
    vector<vector<double>> data;
    void parse_args(std::vector<std::string> &args)
    {
        if (args.size() < 8)
        {
            throw std::runtime_error("Illformed args");
        }
        symbol1 = args[0];
        symbol2 = args[1];
        x = stoi(args[2]);
        n = stoi(args[3]);
        threshold = stod(args[4]);
        stop_loss_threshold = stod(args[5]);
        start = args[6];
        end = args[7];
        cout << "symbol1 " << symbol1 << endl;
        cout << "symbol2 " << symbol2 << endl;
        cout << "x " << x << endl;
        cout << "n " << n << endl;
        cout << "threshold " << threshold << endl;
        cout << "stop_loss_threshold " << stop_loss_threshold << endl;
        cout << "start " << start << endl;
        cout << "end " << end << endl;
    }

    void get_data()
    {
        std::string command1 = "python3 api.py BASIC " + symbol1 + " " + std::to_string(n) + " " + start + " " + end +
                               " pairs_sl_" + symbol1 + ".csv";
        std::string command2 = "python3 api.py BASIC " + symbol2 + " " + std::to_string(n) + " " + start + " " + end +
                               " pairs_sl_" + symbol2 + ".csv";
        system(command1.c_str());
        system(command2.c_str());
        ClosePriceParser csv1(("pairs_sl_" + symbol1 + ".csv").c_str());
        ClosePriceParser csv2(("pairs_sl_" + symbol2 + ".csv").c_str());
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

    Statistics get_stats()
    {
        vector<pair<string, double>> cashflow;
        vector<vector<string>> order_stats;
        vector<vector<string>> order_stats1;
        int idx;
        double total_profit = 0;
        double sum = 0;
        double sum_sq = 0;
        int position = 0;
        double cash_transaction = 0;
        vector<vector<double>> loss_monitor;
        for (idx = 0; idx < n - 1; idx++)
        {
            sum = sum + data[idx][0] - data[idx][1];
            sum_sq = sum_sq + ((data[idx][0] - data[idx][1]) * (data[idx][0] - data[idx][1]));
        }
        int upper = idx;
        int lower = 0;
        while (upper < data.size())
        {
            sum = sum + data[upper][0] - data[upper][1];
            sum_sq = sum_sq + ((data[upper][0] - data[upper][1]) * (data[upper][0] - data[upper][1]));

            double mean = sum / double(n); // Assuming st-0 is in data[0] and st-1 is in data[1]
            double sd = sqrt(((sum_sq) / double(n)) - (mean * mean));
            double spread = data[upper][0] - data[upper][1];
            double z_score = (spread - mean) / sd;
            string s = "none";
            int pos_closed = 0;
            vector<vector<double>> to_be_contained;
            for (int i = 0; i < loss_monitor.size(); i++)
            {
                double new_z_score = (spread - (loss_monitor[i][0])) / loss_monitor[i][1];
                if (new_z_score > stop_loss_threshold)
                {
                    pos_closed++;
                    if (position > 0)
                    {
                        s = "sell";
                        cash_transaction = cash_transaction + data[idx][0] - data[idx][1];
                        cashflow.push_back(make_pair(dates[idx], cash_transaction));
                        vector<string> v = {dates[idx], "SELL", "1", to_string(data[idx][0])};
                        order_stats.push_back(v);
                        vector<string> v1 = {dates[idx], "BUY", "1", to_string(data[idx][1])};
                        order_stats1.push_back(v1);
                    }
                    else
                    {
                        s = "buy";
                        cash_transaction = cash_transaction - data[idx][0] + data[idx][1];
                        cout << "BUYO" << endl;
                        cashflow.push_back(make_pair(dates[idx], cash_transaction));
                        vector<string> v = {dates[idx], "BUY", "1", to_string(data[idx][0])};
                        order_stats.push_back(v);
                        vector<string> v1 = {dates[idx], "SELL", "1", to_string(data[idx][1])};
                        order_stats1.push_back(v1);
                    }
                }
                else
                {
                    to_be_contained.push_back(loss_monitor[i]);
                }
            }
            loss_monitor = to_be_contained;
            position = position - pos_closed;

            if (z_score > threshold)
            {
                if (position > -x)
                {

                    if (position > 0)
                    {
                        double max_val = (spread - (loss_monitor[0][0])) / loss_monitor[0][1];
                        for (int i = 0; i < loss_monitor.size(); i++)
                        {
                            max_val = max(max_val, (spread - (loss_monitor[i][0])) / loss_monitor[i][1]);
                        }
                        int idex = 0;
                        for (int i = 0; i < loss_monitor.size(); i++)
                        {
                            if (((spread - (loss_monitor[i][0])) / loss_monitor[i][1]) == max_val)
                            {
                                idex = i;
                            }
                        }
                        loss_monitor.erase(loss_monitor.begin() + idex);
                    }
                    else
                    {
                        vector<double> v = {mean, sd};
                        loss_monitor.push_back(v);
                    }
                    cash_transaction = cash_transaction + data[upper][0] - data[upper][1];
                    position--;
                    cout << position << endl;
                    cashflow.push_back(make_pair(dates[upper], cash_transaction));
                    vector<string> v = {dates[upper], "SELL", "1", to_string(data[upper][0])};
                    order_stats.push_back(v);
                    vector<string> v1 = {dates[upper], "BUY", "1", to_string(data[upper][1])};
                    order_stats1.push_back(v1);
                }
            }
            else if (z_score < -threshold)
            {
                if (position < x)
                {
                    if (position < 0)
                    {
                        double max_val = (spread - (loss_monitor[0][0])) / loss_monitor[0][1];
                        for (int i = 0; i < loss_monitor.size(); i++)
                        {
                            max_val = max(max_val, (spread - (loss_monitor[i][0])) / loss_monitor[i][1]);
                        }
                        int idex = 0;
                        for (int i = 0; i < loss_monitor.size(); i++)
                        {
                            if (((spread - (loss_monitor[i][0])) / loss_monitor[i][1]) == max_val)
                            {
                                idex = i;
                            }
                        }
                        loss_monitor.erase(loss_monitor.begin() + idex);
                    }
                    else
                    {
                        vector<double> v = {mean, sd};
                        loss_monitor.push_back(v);
                    }
                    cash_transaction = cash_transaction + data[upper][1] - data[upper][0];
                    position++;
                    cout << position << endl;
                    cashflow.push_back(make_pair(dates[upper], cash_transaction));
                    vector<string> v = {dates[upper], "BUY", "1", to_string(data[upper][0])};
                    order_stats.push_back(v);
                    vector<string> v1 = {dates[upper], "SELL", "1", to_string(data[upper][1])};
                    order_stats1.push_back(v1);
                }
            }

            sum = sum - (data[lower][0] - data[lower][1]);

            sum_sq = sum_sq - ((data[lower][0] - data[lower][1]) * (data[lower][0] - data[lower][1]));
            lower++;
            upper++;
        }

        total_profit = cash_transaction + (position * (data.back()[0] - data.back()[1]));
        // cout << total_profit << endl;

        Statistics ans;
        ans.daily_cashflow = cashflow;
        ans.final_pnl = total_profit;
        ans.order_statistics = order_stats;
        ans.order_statistics2 = order_stats1;

        // cout << total_profit << endl;
        return ans;
    }

  public:
    PairsSL(std::vector<std::string> args)
    {
        this->parse_args(args);
        this->get_data();
        this->stats = get_stats();
    };
    ~PairsSL()
    {
        remove(("pairs_sl_" + symbol1 + ".csv").c_str());
        remove(("pairs_sl_" + symbol2 + ".csv").c_str());
    }
};