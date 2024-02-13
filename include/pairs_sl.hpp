#include "csv_parser.hpp"
#include "my_utils.hpp"
#include <cmath>
#include <iostream>
#include <strategy.hpp>
#include <unordered_map>

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
        std::string command1 = "python3 api.py BASIC " + symbol1 + " " + std::to_string(n - 1) + " " + start + " " +
                               end + " pairs_sl_" + symbol1 + ".csv";
        std::string command2 = "python3 api.py BASIC " + symbol2 + " " + std::to_string(n - 1) + " " + start + " " +
                               end + " pairs_sl_" + symbol2 + ".csv";
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
        print(data);
    }

    Statistics get_stats()
    {
        vector<pair<string, double>> cashflow;
        vector<vector<string>> order_stats;
        vector<vector<string>> order_stats1;
        int min_pos = 90;
        // cout<<"In"<<endl;
        double total_profit = 0;
        int position = 0;
        double cash_transaction = 0;
        vector<vector<double>> loss_monitor;
        unordered_map<string, double> buy_sell_stats;

        for (int upper = n - 1; upper < data.size(); upper++)
        {
            cout << dates[upper] << endl;
            // cout<<upper<<endl;
            ////cout<<position<<" "<<loss_monitor.size()<<endl;
            double sum = 0;
            double sum_sq = 0;
            for (int i = upper + 1 - n; i <= upper; i++)
            {
                sum = sum + data[i][0] - data[i][1];
                sum_sq = sum_sq + ((data[i][0] - data[i][1]) * (data[i][0] - data[i][1]));
            }
            double mean = sum / double(n); // Assuming st-0 is in data[0] and st-1 is in data[1]
            double sd = sqrt(((sum_sq) / double(n)) - (mean * mean));
            double spread = data[upper][0] - data[upper][1];
            double z_score = (spread - mean) / sd;
            // cout<<"Z_score"<<z_score<<"->"<<sd<<endl;
            string s = "none";
            int pos_closed = 0;
            vector<vector<double>> to_be_contained;
            // cout<<"PREV"<<loss_monitor.size()<<endl;
            ////////cout<<"The loss_monitor"<<"->";
            // for(int i=0;i<loss_monitor.size();i++){
            //     //////cout<<"("<<loss_monitor[i][0]<<" "<<loss_monitor[i][1]<<")";
            // }//////cout<<endl;

            int number_of_stocks_traded = 0;
            for (int i = 0; i < loss_monitor.size(); i++)
            {
                double new_z_score = (spread - (loss_monitor[i][0])) / loss_monitor[i][1];
                //////cout<<"("<<loss_monitor[i][0]<<"->"<<loss_monitor[i][1]<<"->"<<new_z_score;
                if (new_z_score > stop_loss_threshold)
                {
                    pos_closed++;
                    if (position > 0)
                    {
                        s = "sell";
                        cout << "sello" << endl;
                        cash_transaction = cash_transaction + data[upper][0] - data[upper][1];
                        number_of_stocks_traded++;
                        vector<string> v = {dates[upper], "SELL", "1", to_string(data[upper][0])};
                        order_stats.push_back(v);
                        vector<string> v1 = {dates[upper], "BUY", "1", to_string(data[upper][1])};
                        order_stats1.push_back(v1);
                    }
                    else
                    {
                        s = "buy";
                        cash_transaction = cash_transaction - data[upper][0] + data[upper][1];
                        cout << "BUYO" << endl;
                        number_of_stocks_traded++;
                        vector<string> v = {dates[upper], "BUY", "1", to_string(data[upper][0])};
                        order_stats.push_back(v);
                        vector<string> v1 = {dates[upper], "SELL", "1", to_string(data[upper][1])};
                        order_stats1.push_back(v1);
                    }
                }
                else
                {
                    to_be_contained.push_back(loss_monitor[i]);
                }
            }
            loss_monitor = to_be_contained;
            if (position > 0)
            {
                position = position - pos_closed;
            }
            else if (position < 0)
            {
                position = position + pos_closed;
            }

            //////cout<<"z_score : "<<z_score<<endl;
            if (z_score > threshold)
            {
                // cout<<"SELL"<<loss_monitor.size()<<endl;
                if (position > -x)
                {
                    cout << "sell" << endl;
                    //////cout<<"SELL"<<endl;
                    if (s == "none")
                    {

                        if (position > 0)
                        {
                            loss_monitor.erase(loss_monitor.begin() + 0);
                        }
                        else
                        {
                            vector<double> v = {mean, sd};
                            loss_monitor.push_back(v);
                        }
                        cash_transaction = cash_transaction + data[upper][0] - data[upper][1];
                        position--;
                        //////////cout << position << endl;

                        vector<string> v = {dates[upper], "SELL", "1", to_string(data[upper][0])};
                        order_stats.push_back(v);
                        vector<string> v1 = {dates[upper], "BUY", "1", to_string(data[upper][1])};
                        order_stats1.push_back(v1);
                    }
                    else if (s == "sell")
                    {
                        for (int i = 0; i < number_of_stocks_traded; i++)
                        {
                            order_stats.pop_back();
                            order_stats1.pop_back();
                        }
                        if (position > 0)
                        {

                            loss_monitor.erase(loss_monitor.begin() + 0);
                        }
                        else
                        {
                            vector<double> v = {mean, sd};
                            loss_monitor.push_back(v);
                        }
                        cash_transaction = cash_transaction + data[upper][0] - data[upper][1];
                        position--;
                        vector<string> v = {dates[upper], "SELL", to_string(number_of_stocks_traded + 1),
                                            to_string(data[upper][0])};
                        order_stats.push_back(v);
                        vector<string> v1 = {dates[upper], "BUY", to_string(number_of_stocks_traded + 1),
                                             to_string(data[upper][1])};
                        order_stats1.push_back(v1);
                    }
                    else if (s == "buy")
                    {
                        for (int i = 0; i < number_of_stocks_traded; i++)
                        {
                            order_stats.pop_back();
                            order_stats1.pop_back();
                        }
                        if (position > 0)
                        {

                            loss_monitor.erase(loss_monitor.begin() + 0);
                        }
                        else
                        {
                            vector<double> v = {mean, sd};
                            loss_monitor.push_back(v);
                        }
                        cash_transaction = cash_transaction + data[upper][0] - data[upper][1];
                        position--;
                        if (number_of_stocks_traded - 1 > 0)
                        {
                            vector<string> v = {dates[upper], "BUY", to_string(number_of_stocks_traded - 1),
                                                to_string(data[upper][0])};
                            order_stats.push_back(v);
                            vector<string> v1 = {dates[upper], "SELL", to_string(number_of_stocks_traded - 1),
                                                 to_string(data[upper][1])};
                            order_stats1.push_back(v1);
                        }
                    }
                }
            }
            else if (z_score < -threshold)
            {

                //////cout<<"BUY"<<endl;
                if (position < x)
                {

                    if (s == "none")
                    {

                        if (position < 0)
                        {
                            // cout<<"position < 0"<<loss_monitor.size()<<endl;
                            loss_monitor.erase(loss_monitor.begin() + 0);
                            // cout<<"position < 0"<<loss_monitor.size()<<endl;
                        }
                        else
                        {
                            vector<double> v = {mean, sd};
                            loss_monitor.push_back(v);
                        }
                        cash_transaction = cash_transaction + data[upper][1] - data[upper][0];
                        position++;
                        //////////cout << position << endl;

                        vector<string> v = {dates[upper], "BUY", "1", to_string(data[upper][0])};
                        order_stats.push_back(v);
                        vector<string> v1 = {dates[upper], "SELL", "1", to_string(data[upper][1])};
                        order_stats1.push_back(v1);
                    }
                    else if (s == "buy")
                    {
                        // cout<<"buy"<<loss_monitor.size()<<endl;
                        for (int i = 0; i < number_of_stocks_traded; i++)
                        {
                            order_stats.pop_back();
                            order_stats1.pop_back();
                        }
                        if (position < 0)
                        {

                            loss_monitor.erase(loss_monitor.begin() + 0);
                        }
                        else
                        {
                            vector<double> v = {mean, sd};
                            loss_monitor.push_back(v);
                        }
                        cash_transaction = cash_transaction + data[upper][1] - data[upper][0];
                        position++;
                        //////////cout << position << endl;
                        vector<string> v = {dates[upper], "BUY", to_string(number_of_stocks_traded + 1),
                                            to_string(data[upper][0])};
                        order_stats.push_back(v);
                        vector<string> v1 = {dates[upper], "SELL", to_string(number_of_stocks_traded + 1),
                                             to_string(data[upper][1])};
                        order_stats1.push_back(v1);
                    }
                    else if (s == "sell")
                    {
                        // cout<<"sell"<<loss_monitor.size()<<endl;
                        for (int i = 0; i < number_of_stocks_traded; i++)
                        {
                            order_stats.pop_back();
                            order_stats1.pop_back();
                        }
                        if (position < 0)
                        {

                            loss_monitor.erase(loss_monitor.begin() + 0);
                        }
                        else
                        {
                            vector<double> v = {mean, sd};
                            loss_monitor.push_back(v);
                        }
                        cash_transaction = cash_transaction + data[upper][1] - data[upper][0];
                        position++;
                        //////////cout << position << endl;

                        if (number_of_stocks_traded - 1 > 0)
                        {
                            vector<string> v = {dates[upper], "BUY", to_string(number_of_stocks_traded - 1),
                                                to_string(data[upper][0])};
                            order_stats.push_back(v);
                            vector<string> v1 = {dates[upper], "SELL", to_string(number_of_stocks_traded - 1),
                                                 to_string(data[upper][1])};
                            order_stats1.push_back(v1);
                        }
                    }
                }
            }

            //////cout<<"order_stats : "<<endl;
            for (int i = 0; i < order_stats.size(); i++)
            {
                //////cout<<order_stats[i][0]<<" "<<order_stats[i][1]<<" "<<order_stats[i][2]<<endl;
            }
            cashflow.push_back(make_pair(dates[upper], cash_transaction));
        }

        total_profit = cash_transaction + (position * (data.back()[0] - data.back()[1]));
        ////////////cout << total_profit << endl;
        Statistics ans;
        ans.daily_cashflow = cashflow;
        ans.final_pnl = total_profit;
        cout << total_profit << "\n";
        ans.order_statistics = order_stats;
        ans.order_statistics2 = order_stats1;
        for (int i = 0; i < order_stats.size(); i++)
        {
            //////////cout<<order_stats[i][0]<<"->"<<order_stats[i][1]<<" "<<order_stats[i][2]<<"
            ///"<<order_stats[i][3]<<endl;
        }
        for (int i = 0; i < order_stats1.size(); i++)
        {
            cout << order_stats1[i][1] << stoi(order_stats1[i][3]) << " " << order_stats[i][1] << order_stats[i][3]
                 << " " << stoi(order_stats[i][3]) - stoi(order_stats1[i][3]) << endl;
        }
        // cout<<"OUT"<<endl;
        cout << total_profit << endl;
        return ans;
    }

  public:
    PairsSL(std::vector<std::string> args)
    {
        this->parse_args(args);
        this->get_data();
        print(this->data);
        this->stats = get_stats();
    };
    ~PairsSL()
    {
        // remove(("pairs_sl_" + symbol1 + ".csv").c_str());
        // remove(("pairs_sl_" + symbol2 + ".csv").c_str());
    }
};
