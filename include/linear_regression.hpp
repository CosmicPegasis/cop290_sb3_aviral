#include "matrix_util.hpp"
#include <algorithm>
#include <csv_parser.hpp>
#include <exception>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <strategy.hpp>
#include <vector>

#pragma once
class LinearRegressionCore
{
    static double dot_product(const std::vector<double> &v1, const std::vector<double> &v2)
    {
        if (v1.size() != v2.size())
            throw std::runtime_error("v1 and v2 don't have same dimensions.");

        return inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
    }

    std::vector<double> theta;
    std::vector<std::vector<double>> x_vec;
    std::vector<double> y_vec;
    bool trained = false;

    std::vector<double> calculte_theta()
    {
        const std::vector<std::vector<double>> x = x_vec;
        std::vector<std::vector<double>> y(y_vec.size(), std::vector<double>());
        for (int i = 0; i < y_vec.size(); i++)
        {
            y[i].push_back(y_vec[i]);
        }
        const std::vector<std::vector<double>> xt = MatrixUtil::transpose(x);
        const std::vector<std::vector<double>> xxt_inv = MatrixUtil::inverse(MatrixUtil::product(xt, x));
        const std::vector<std::vector<double>> xty = MatrixUtil::product(xt, y);

        return MatrixUtil::transpose(MatrixUtil::product(xxt_inv, xty))[0];
    };

  public:
    LinearRegressionCore(const std::vector<std::vector<double>> &x, const std::vector<double> &y) : x_vec{x}, y_vec{y}
    {
        if (x.size() != y.size())
            throw std::runtime_error("x and y don't have same dimensions");

        if (x.size() == 0)
            throw std::runtime_error("cannot pass empty dataset");
        theta = calculte_theta();
        print(theta);
        this->trained = true;
    }
    LinearRegressionCore(){};

    double predict(const std::vector<double> &x)
    {
        if (this->trained)
        {
            return dot_product(x, theta);
        }
        else
        {
            throw std::runtime_error("model has not been trained");
        }
    }
    std::vector<double> get_theta()
    {
        if (this->trained)
        {
            return theta;
        }
        else
        {
            throw std::runtime_error("model has not been trained");
        }
    }
};

struct Dataset
{
    std::vector<std::vector<double>> in_params;
    std::vector<double> close_price;
    std::vector<std::string> dates;
};
class LinearRegression : public Strategy
{
    int x, p;
    std::string symbol, train_start, train_end, start, end;
    LinearRegressionCore core;
    static std::vector<std::vector<double>> get_transformed(const std::vector<std::vector<double>> &x)
    {
        // We need to apppend all the x's by ones becuase we also need a correct theta_not (constant term)
        std::vector<std::vector<double>> x_with_constant_term = x;
        for (auto &elem : x_with_constant_term)
        {
            elem.push_back(1);
        }
        return x_with_constant_term;
    }

    // TODO Add destructor
    void query_exchange()
    {
        std::string command = "python3 api.py LINEAR_REGRESSION " + symbol + " " + train_start + " " + train_end + " " +
                              start + " " + end + " regression";
        system(command.c_str());
    }
    static Dataset get_data(std::string filename)
    {
        CSVParser csv(filename);
        Dataset result;
        result.dates = csv.get_column(0);
        std::vector<std::vector<std::string>> csv_in_params;
        result.in_params = std::vector<std::vector<double>>(result.dates.size(), std::vector<double>(7, 0));
        for (int j = 2; j < 9; j++)
        {
            std::vector<std::string> csv_column = csv.get_column(j);
            for (int i = 0; i < csv_column.size(); i++)
            {
                result.in_params[i][j - 2] = stod(csv_column[i]);
            }
        }
        std::vector<std::string> csv_close_price = csv.get_column(1);
        for (auto &elem : csv_close_price)
        {
            result.close_price.push_back(stod(elem));
        }
        return result;
    };

    void initialise_core(const std::vector<std::vector<double>> &x, const std::vector<double> &y)
    {

        std::vector<std::vector<double>> x_with_constant_term = get_transformed(x);
        this->core = std::move(LinearRegressionCore{x_with_constant_term, y});
    }
    double predict(std::vector<double> x)
    {
        x.push_back(1);
        return core.predict(x);
    }
    std::vector<double> get_predictions(const std::vector<std::vector<double>> &x)
    {
        std::vector<double> result;
        for (auto &elem : x)
        {
            result.push_back(this->predict(elem));
        }
        return result;
    }
    Statistics get_stats(std::vector<double> predicted_prices, std::vector<double> close_prices, int p, int x)
    {
        return Statistics{};
    };
    void parse_args(std::vector<std::string> args)
    {
        // LinearRegression(std::string symbol, int x, int p, std::string train_start, std::string train_end,
        //                  std::string start, std::string end)
        if (args.size() < 7)
        {
            throw std::runtime_error("Illformed argument list");
        }
        this->symbol = args[0];
        this->x = stoi(args[1]);
        this->p = stoi(args[2]);
        this->train_start = args[3];
        this->train_end = args[4];
        this->start = args[5];
        this->end = args[6];
    }

    Statistics get_stats(std::vector<double> &predicted_prices, std::vector<double> &close_price,
                         std::vector<std::string> &date)
    {
        std::vector<std::pair<std::string, double>> cashflow;
        std::vector<std::vector<std::string>> order_stats;
        double total_profit = 0;
        int position = 0;
        double cash_transaction = 0;

        for (int i = 0; i < predicted_prices.size(); i++)
        {
            if (predicted_prices[i] > close_price[i] + ((double(p) * close_price[i]) / 100))
            {
                if (position < x)
                {
                    cash_transaction = cash_transaction - close_price[i];
                    position++;
                    
                    std::vector<std::string> v = {date[i], "BUY", "1", std::to_string(close_price[i])};
                    order_stats.push_back(v);
                }
            }
            else if (predicted_prices[i] < close_price[i] - ((double(p) * close_price[i]) / 100))
            {
                if (position > -x)
                {
                    cash_transaction = cash_transaction + close_price[i];
                    position--;
                    
                    std::vector<std::string> v = {date[i], "SELL", "1", std::to_string(close_price[i])};
                    order_stats.push_back(v);
                }
            }
            cashflow.push_back(std::make_pair(date[i], cash_transaction));
        }

        total_profit = cash_transaction + (position * close_price.back());

        Statistics ans;
        ans.daily_cashflow = cashflow;
        ans.final_pnl = total_profit;
        ans.order_statistics = order_stats;
        return ans;
    }

  public:
    LinearRegression(const std::vector<std::vector<double>> &x, const std::vector<double> &y)
    {
        this->initialise_core(x, y);
    }
    LinearRegression(std::vector<std::string> args)
    {
        this->parse_args(args);
        this->query_exchange();
        Dataset training_data = this->get_data("train_regression.csv");
        this->initialise_core(training_data.in_params, training_data.close_price);
        Dataset trade_data = this->get_data("trade_regression.csv");
        std::vector<double> predicted_prices = this->get_predictions(trade_data.in_params);
        this->stats = get_stats(predicted_prices, trade_data.close_price, trade_data.dates);
    }
    ~LinearRegression()
    {
        remove("trade_regression.csv");
        remove("train_regression.csv");
    }
};