#include "adx.hpp"
#include <cmath>
#include <csv_parser.hpp>
#include <iostream>
#include <strategy.hpp>

using namespace std;
void ADX::parse_args(std::vector<std::string> &args)
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

void ADX::get_data()
{
    std::string command =
        "python3 api.py ADX " + symbol + " " + std::to_string(1) + " " + start + " " + end + " adx.csv";
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

Statistics ADX::get_stats(vector<vector<double>> data, vector<string> dates)
{

    vector<pair<string, double>> cashflow;
    vector<vector<string>> order_stats;
    double total_profit = 0;
    double cash_transaction = 0;
    int position = 0;
    double ATR = 0;
    double DI_plus = 0;
    double DI_minus = 0;
    double ADX = 0;

    double alpha = double(2) / double(n + 1);
    cout << alpha << endl;
    for (int idx = 1; idx < data.size(); idx++)
    {
        double DX = 0;
        double DM_plus;
        double DM_minus;
        double TR =
            max(data[idx][1] - data[idx][2], max(data[idx][1] - data[idx - 1][0], data[idx][2] - data[idx - 1][0]));
        if (idx == 1)
        {
            ATR = TR;
            DM_plus = max(0.0, data[idx][1] - data[idx - 1][1]);
            DM_minus = max(0.0, data[idx][2] - data[idx - 1][2]);
            DI_plus = DI_plus / ATR;
            DI_minus = DI_minus / ATR;
            if ((DI_plus + DI_minus) == 0)
            {
                DX = 0;
            }
            else
            {
                DX = 100.0 * (DI_plus - DI_minus) / (DI_plus + DI_minus);
            }
            ADX = DX;
        }
        else
        {
            ATR = ATR + (alpha * (TR - ATR));
            DM_plus = max(0.0, data[idx][1] - data[idx - 1][1]);
            DI_plus = DI_plus + (alpha * ((DM_plus / ATR) - DI_plus));
            DM_minus = max(0.0, data[idx][2] - data[idx - 1][2]);
            DI_minus = DI_minus + (alpha * ((DM_minus / ATR) - DI_minus));
            if ((DI_plus + DI_minus) == 0)
            {
                DX = 0;
            }
            else
            {
                DX = 100.0 * (DI_plus - DI_minus) / (DI_plus + DI_minus);
            }
            ADX = ADX + (alpha * (DX - ADX));
        }
        // cout<<date[idx]<<"->"<<ADX<<"->"<<DI_plus<<"->"<<DI_minus<<"->"<<DM_minus<<"->"<<DM_plus<<"->"<<ATR<<"->"<<TR<<endl;
        if (DX != 0)
        {
            if (ADX > adx_threshold)
            {
                if (position < x)
                {
                    cash_transaction = cash_transaction - data[idx][0];
                    position++;
                    cout << dates[idx] << "->"
                         << "BUY" << endl;
                    vector<string> v = {dates[idx], "BUY", "1", to_string(data[idx][0])};
                    order_stats.push_back(v);
                }
            }
            if (ADX < adx_threshold)
            {
                if (position > -x)
                {
                    cash_transaction = cash_transaction + data[idx][0];
                    position--;
                    cout << dates[idx] << "->"
                         << "SELL" << endl;
                    vector<string> v = {dates[idx], "SELL", "1", to_string(data[idx][0])};
                    order_stats.push_back(v);
                }
            }
        }

        cashflow.push_back(make_pair(dates[idx], cash_transaction));
    }

    total_profit = cash_transaction + (position * data.back()[0]);

    Statistics ans;
    ans.daily_cashflow = cashflow;
    ans.final_pnl = total_profit;
    ans.order_statistics = order_stats;

    cout << total_profit << endl;
    return ans;
}

ADX::ADX(std::vector<std::string> args)
{
    this->parse_args(args);
    this->get_data();
    this->stats = get_stats(data, dates);
};
ADX::~ADX()
{
    remove("adx.csv");
}