#include <basic.hpp>
#include <cmath>
#include <iostream>
#include <strategy.hpp>

#pragma once
using namespace std;
class MACD : public Strategy
{
    int n, x, p, max_hold_days;
    double c1, c2;
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<double> close_prices;
    void parse_args(std::vector<std::string> &args)
    {
        if (args.size() < 4)
        {
            throw std::runtime_error("Illformed args");
        }
        symbol = args[0];
        x = stoi(args[1]);
        start = args[2];
        end = args[3];
        cout << "symbol " << symbol << endl;
        cout << "x " << x << endl;
        cout << "start " << start << endl;
        cout << "end " << end << endl;
    }

    void get_data()
    {
        std::string command = "python3 api.py MACD " + symbol + " " + "34" + " " + start + " " + end + " macd.csv";
        system(command.c_str());
        ClosePriceParser csv("macd.csv");
        this->dates = csv.get_dates();
        this->close_prices = csv.get_close_price();
    }

    void get_ewm(vector<double> &ewm_list, vector<double> prices, double aplha)
    {
        ewm_list = {prices[0]};
        for (int i = 1; i < prices.size(); i++)
        {
            double ewm = aplha * (prices[i] - ewm_list.back()) + ewm_list.back();
            ewm_list.push_back(ewm);
        }
    }

    string macd_strategy(vector<double> close_price)
    {
        double short_ewm_alpha = double(2) / double(12 + 1);
        double long_ewm_alpha = double(2) / double(26 + 1);
        double signal_line_alpha = double(2) / double(9 + 1);
        vector<double> short_ewm;
        vector<double> long_ewm;
        vector<double> macd_vector(9);
        vector<double> signal_line;
        int idx = 34;
        vector<double> price_12days(close_price.begin() + idx - 12, close_price.begin() + idx);
        // //cout << "past12days ";

        for (int i = 0; i < price_12days.size(); i++)
        {
            // //cout << price_12days[i] << " ";
        }

        // //cout << endl;

        get_ewm(short_ewm, price_12days, short_ewm_alpha);

        vector<double> price_26days(close_price.begin() + idx - 26, close_price.begin() + idx);

        get_ewm(long_ewm, price_26days, long_ewm_alpha);
        // cout << "past26days ";

        for (int i = 0; i < price_26days.size(); i++)
        {
            // cout << price_26days[i] << " ";
        }

        // cout << endl;
        // cout << "short_ewm";
        for (int i = 0; i < short_ewm.size(); i++)
        {
            // cout << short_ewm[i] << " ";
        }
        // cout << endl;
        // cout << "long_ewm" << endl;
        for (int i = 0; i < long_ewm.size(); i++)
        {
            // cout << long_ewm[i] << " ";
        }
        // cout << endl;

        double macd = short_ewm.back() - long_ewm.back();
        idx = 34;

        for (int i = 0; i < 9; i++)
        {
            vector<double> price_12days(close_price.begin() + idx - 12, close_price.begin() + idx);
            get_ewm(short_ewm, price_12days, short_ewm_alpha);
            vector<double> price_26days(close_price.begin() + idx - 26, close_price.begin() + idx);
            get_ewm(long_ewm, price_26days, long_ewm_alpha);
            double macd_helper = short_ewm.back() - long_ewm.back();
            macd_vector[9 - i - 1] = macd_helper;
            idx--;
        }
        // calculate_macd(short_ewm, long_ewm, macd_vector);
        // cout << "macd_vector ";
        for (int i = 0; i < macd_vector.size(); i++)
        {
            // cout << macd_vector[i] << " ";
        }
        // cout << endl;
        get_ewm(signal_line, macd_vector, signal_line_alpha);

        // cout << "signal_line" << endl;
        for (int i = 0; i < signal_line.size(); i++)
        {
            // cout << signal_line[i] << " ";
        }
        // cout << endl;

        if (macd > signal_line.back())
        {
            // cout << "BUY" << endl;
            return "BUY";
        }
        else
        {
            // cout << "SELL" << endl;
            return "SELL";
        }
    }
    Statistics macd(vector<double> close_price, vector<string> date, int x)
    {

        vector<pair<string, double>> cashflow;
        vector<vector<string>> order_stats;
        int position = 0;
        int idx;
        double cash_transaction = 0;
        for (idx = 33; idx < close_price.size(); idx++)
        {
            // cout << date[idx] << " ";

            vector<double> v;
            for (int i = idx - 33; i <= idx; i++)
            {
                v.push_back(close_price[i]);
            }
            if (macd_strategy(v) == "BUY")
            {
                if (position < x)
                {
                    cash_transaction = cash_transaction - close_price[idx];
                    position++;
                    // cout << position << endl;
                    cashflow.push_back(make_pair(date[idx], cash_transaction));
                    vector<string> v = {date[idx], "BUY", "1", to_string(close_price[idx])};
                    order_stats.push_back(v);
                }
            }
            else
            {
                if (position > -x)
                {
                    cash_transaction = cash_transaction + close_price[idx];
                    position--;
                    // cout << position << endl;
                    cashflow.push_back(make_pair(date[idx], cash_transaction));
                    vector<string> v = {date[idx], "SELL", "1", to_string(close_price[idx])};
                    order_stats.push_back(v);
                }
            }
        }
        double total_profit = 0;
        total_profit = cash_transaction + (position * close_price.back());
        // //cout << total_profit << endl;

        Statistics ans;
        ans.daily_cashflow = cashflow;
        ans.final_pnl = total_profit;
        ans.order_statistics = order_stats;

        // cout << total_profit << endl;
        return ans;
    }

    Statistics get_stats(vector<vector<double>> data, vector<string> date)
    {
        vector<double> close_price;
        for (int i = 0; i < data.size(); i++)
        {
            close_price.push_back(data[i][0]);
        }
        return macd(close_price, date, x);
    }

  public:
    MACD(std::vector<std::string> args)
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
    ~MACD()
    {
        remove("macd.csv");
    }
};