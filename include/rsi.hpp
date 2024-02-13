#include <cmath>
#include <csv_parser.hpp>
#include <iostream>
#include <strategy.hpp>

#pragma once
using namespace std;
class RSI : public Strategy
{
    int n, x;
    double oversold, overbought;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args)
    {
        if (args.size() < 7)
        {
            throw std::runtime_error("Illformed args");
        }
        symbol = args[0];
        x = stoi(args[1]);
        n = stoi(args[2]);
        oversold = stod(args[3]);
        overbought = stod(args[4]);
        start = args[5];
        end = args[6];
        cout << "symbol " << symbol << endl;
        cout << "x " << x << endl;
        cout << "n " << n << endl;
        cout << "oversold_threshold " << oversold << endl;
        cout << "overbought_threshold " << overbought << endl;
        cout << "start " << start << endl;
        cout << "end " << end << endl;
    }

    void get_data()
    {
        std::string command =
            "python3 api.py RSI " + symbol + " " + std::to_string(n) + " " + start + " " + end + " rsi.csv";
        system(command.c_str());
        ClosePriceParser csv("rsi.csv");
        this->dates = csv.get_dates();
        this->close_prices = csv.get_close_price();
    }

    Statistics get_stats(vector<vector<double>> data, vector<string> date)
    {
        vector<pair<string, double>> cashflow;
    vector<vector<string>> order_stats;
    int idx;
    int position = 0;
    double gain = 0;
    double loss = 0;
    double total_profit = 0;
    double cash_transaction = 0;
    for (idx = 1; idx <= n; idx++)
    {
        if (data[idx][0] - data[idx - 1][0] >= 0)
        {
            gain = gain + data[idx][0] - data[idx - 1][0];
        }
        else
        {
            loss = loss + data[idx - 1][0] - data[idx][0];
        }
    }
    int lower = 0;
    int upper = idx-1;
    //cout << upper << endl;
    //cout << data.size() << endl;
    while (upper < data.size())
    {
        double rs = gain / loss;
        double rsi = double(100) - double(100) / double(1 + rs);

        if (rsi < oversold)
        {
            if (position < x)
            {
                cash_transaction = cash_transaction - data[upper][0];
                position++;
                //cout << position << endl;
                
                vector<string> v = {date[upper], "BUY", "1", to_string(data[upper][0])};
                order_stats.push_back(v);
            }
        }
        if (rsi > overbought)
        {
            if (position > -x)
            {
                cash_transaction = cash_transaction + data[upper][0];
                position--;
                //cout << position << endl;
                
                vector<string> v = {date[upper], "SELL", "1", to_string(data[upper][0])};
                order_stats.push_back(v);
            }
        }
        cashflow.push_back(make_pair(date[upper], cash_transaction));
        if(upper != data.size()-1){
            double change1 = data[upper+1][0] - data[upper ][0];
            double change2 = data[lower + 1][0] - data[lower][0];
            //cout << "change1->" << change1 << "change2->" << change2 << endl;
            if (change1 >= 0)
            {
                gain = gain + change1;
            }
            else
            {
                loss = loss - change1;
            }
            if (change2 >= 0)
            {
                gain = gain - change2;
            }
            else
            {
                loss = loss + change2;
            }
        }
        //cout << date[upper] << "->" << rsi << " " << gain << " " << loss << endl;
             
        upper++;
        lower++;
    }
    
    total_profit = cash_transaction + (position * data.back()[0]);
    //cout << total_profit << endl;

    Statistics ans;
    ans.daily_cashflow = cashflow;
    ans.final_pnl = total_profit;
    ans.order_statistics = order_stats;
    

    cout << total_profit << endl;
    return ans;
    }

  public:
    RSI(std::vector<std::string> args)
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
    ~RSI()
    {
        remove("rsi.csv");
    }
};