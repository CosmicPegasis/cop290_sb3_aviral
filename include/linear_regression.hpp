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

    static Dataset get_training_data(std::string symbol, std::string train_start, std::string train_end)
    {
        system("echo hello world");
        return Dataset{};
    };

    static Dataset get_trade_data(std::string symbol, std::string start, std::string end)
    {
        return Dataset{};
    };
    void initialise_core(const std::vector<std::vector<double>> &x, const std::vector<double> &y)
    {

        std::vector<std::vector<double>> x_with_constant_term = get_transformed(x);
        print(x_with_constant_term);
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
    Statistics get_stats(std::vector<double> predicted_prices, std::vector<double> close_prices, int p, int x){};

  public:
    LinearRegression(const std::vector<std::vector<double>> &x, const std::vector<double> &y)
    {
        this->initialise_core(x, y);
    }
    LinearRegression(std::string symbol, int x, int p, std::string train_start, std::string train_end,
                     std::string start, std::string end)
    {
        Dataset training_data = this->get_training_data(symbol, train_start, train_end);
        this->initialise_core(training_data.in_params, training_data.close_price);

        Dataset trade_data = this->get_trade_data(symbol, start, end);
        std::vector<double> predicted_prices = this->get_predictions(trade_data.in_params);
        this->stats = get_stats(predicted_prices, trade_data.close_price, p, x);
    }
};