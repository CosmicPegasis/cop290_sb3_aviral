#include "strategy.hpp"
#include <iostream>
#include <vector>

#pragma once
class LinearRegressionCore
{
    static double dot_product(const std::vector<double> &v1, const std::vector<double> &v2);

    std::vector<double> theta;
    std::vector<std::vector<double>> x_vec;
    std::vector<double> y_vec;
    bool trained = false;

    std::vector<double> calculte_theta();

  public:
    LinearRegressionCore(const std::vector<std::vector<double>> &x, const std::vector<double> &y);
    LinearRegressionCore();

    double predict(const std::vector<double> &x);
    std::vector<double> get_theta();
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
    static std::vector<std::vector<double>> get_transformed(const std::vector<std::vector<double>> &x);

    // TODO Add destructor
    void query_exchange();
    static Dataset get_data(std::string filename);

    void initialise_core(const std::vector<std::vector<double>> &x, const std::vector<double> &y);
    double predict(std::vector<double> x);
    std::vector<double> get_predictions(const std::vector<std::vector<double>> &x);
    Statistics get_stats(std::vector<double> predicted_prices, std::vector<double> close_prices, int p, int x);
    void parse_args(std::vector<std::string> args);

    Statistics get_stats(std::vector<double> &predicted_prices, std::vector<double> &close_price,
                         std::vector<std::string> &date);

  public:
    LinearRegression(const std::vector<std::vector<double>> &x, const std::vector<double> &y);
    LinearRegression(std::vector<std::string> args);
    LinearRegression(std::string symbol, int x, int p, std::string start, std::string end);

    ~LinearRegression();
};