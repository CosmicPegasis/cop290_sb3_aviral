#include "my_utils.hpp"
#include <cmath>
#include <csv_parser.hpp>
#include <iostream>
#include <strategy.hpp>

#pragma once
using namespace std;
class ADX : public Strategy
{
    int n, x;
    double adx_threshold;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<std::vector<double>> data;
    void parse_args(std::vector<std::string> &args)
    {
        if (args.size() < 6)
        {
            throw std::runtime_error("Illformed args");
        }
        symbol = args[0];
        x = stoi(args[1]);
        n = stoi(args[2]);
        adx_threshold = stod(args[3]);
        start = args[4];
        end = args[5];
        cout << "symbol " << symbol << endl;
        cout << "x " << x << endl;
        cout << "n " << n << endl;
        cout << "adx_threshold " << adx_threshold << endl;
        cout << "start " << start << endl;
        cout << "end " << end << endl;
    }

    void get_data()
    {
        std::string command =
            "python3 api.py ADX " + symbol + " " + std::to_string(3 * n - 2) + " " + start + " " + end + " adx.csv";
        system(command.c_str());
        CSVParser csv("adx.csv");
        this->dates = csv.get_column(0);
        vector<string> close_prices_csv = csv.get_column(1);
        vector<string> high_csv = csv.get_column(2);
        vector<string> low_csv = csv.get_column(3);
        data = vector<vector<double>>(close_prices_csv.size(), vector<double>(3, 0));
        for (int i = 0; i < close_prices_csv.size(); i++)
        {
            this->data[i] = vector<double>{stod(close_prices_csv[i]), stod(high_csv[i]), stod(low_csv[i])};
        }
    }
    Statistics get_stats(vector<vector<double>> close_price, vector<string> dates)
    {
        vector<pair<string, double>> cashflow;
        vector<vector<string>> order_stats;
        double total_profit = 0;
        double cash_transaction = 0;
        int position = 0;
        int idx = (3 * n) - 2;
        double alpha = double(2) / double(n + 1);
        vector<double> ATR;
        vector<double> DI_plus;
        vector<double> DI_minus;
        // cout<<data[1][1]-data[1][2]<<endl;
        for (int i = idx - (2 * n) + 2; i < close_price.size(); i++)
        {

            double ATR_val = 0;
            for (int j = i - n + 1; j <= i; j++)
            {
                if (j == i - n + 1)
                {
                    ATR_val =
                        max(close_price[j][1] - close_price[j][2],
                            max(close_price[j][1] - close_price[j - 1][0], close_price[j][2] - close_price[j - 1][0]));
                }
                else
                {
                    ATR_val =
                        ATR_val +
                        ((max(close_price[j][1] - close_price[j][2], max(close_price[j][1] - close_price[j - 1][0],
                                                                         close_price[j][2] - close_price[j - 1][0])) -
                          ATR_val) *
                         alpha);
                }
            }
            ATR.push_back(ATR_val);
            // cout<<ATR_val<<"->"<<date[i]<<endl;
        }

        for (int i = n - 1; i < ATR.size(); i++)
        {
            double DI_plus_val = 0;
            double DI_minus_val = 0;
            int idex = (i - n + 1) + n;
            for (int j = i - n + 1; j <= i; j++)
            {
                if (j == i - n + 1)
                {

                    DI_plus_val = (max(0.0, close_price[idex][1] - close_price[idex - 1][1])) / ATR[j];
                    DI_minus_val = (max(0.0, close_price[idex][2] - close_price[idex - 1][2])) / ATR[j];
                }
                else
                {
                    DI_plus_val =
                        DI_plus_val + (alpha * (((max(0.0, close_price[idex][1] - close_price[idex - 1][1])) / ATR[j]) -
                                                DI_plus_val));
                    DI_minus_val = DI_minus_val +
                                   (alpha * (((max(0.0, close_price[idex][2] - close_price[idex - 1][2])) / ATR[j]) -
                                             DI_minus_val));
                }
                idex++;
            }
            DI_plus.push_back(DI_plus_val);
            DI_minus.push_back(DI_minus_val);
            // cout<<DI_minus_val<<"->"<<date[(2*n)-1+i-(n-1)]<<endl;
        }

        for (int i = idx; i < close_price.size(); i++)
        {

            double ADX = 0;
            int iterator = i - idx;
            for (int k = 0; k < n; k++)
            {
                if (k == 0)
                {
                    ADX = 100.0 * (DI_plus[iterator] - DI_minus[iterator]) / (DI_plus[iterator] + DI_minus[iterator]);
                }
                else
                {
                    ADX = ADX + (alpha * ((100.0 * (DI_plus[iterator] - DI_minus[iterator]) /
                                           (DI_plus[iterator] + DI_minus[iterator])) -
                                          ADX));
                }
                iterator++;
            }
            // cout<<ADX<<"->"<<date[i]<<endl;
            if (ADX > adx_threshold)
            {
                if (position < x)
                {
                    cash_transaction = cash_transaction - close_price[i][0];
                    position++;
                    // cout << position << endl;
                    // cashflow.push_back(make_pair(dates[i], cash_transaction));
                    vector<string> v = {dates[i], "BUY", "1", to_string(close_price[i][0])};
                    order_stats.push_back(v);
                }
            }
            else if (ADX < adx_threshold)
            {
                if (position > -x)
                {
                    cash_transaction = cash_transaction + close_price[i][0];
                    position--;
                    // cout << position << endl;
                    // cashflow.push_back(make_pair(dates[i], cash_transaction));
                    vector<string> v = {dates[i], "SELL", "1", to_string(close_price[i][0])};
                    order_stats.push_back(v);
                }
            }
            // cout<<ADX<<"->"<<date[i]<<endl;
            // cout << ADX << " " << adx_threshold << endl;
            cashflow.push_back(make_pair(dates[i], cash_transaction));
        }

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
    ADX(std::vector<std::string> args)
    {
        this->parse_args(args);
        this->get_data();
        this->stats = get_stats(data, dates);
    };
    ~ADX()
    {
        remove("adx.csv");
    }
};