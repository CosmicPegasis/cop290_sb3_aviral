#include "matrix_util.hpp"
#include <algorithm>
#include <cstddef>
#include <csv_parser.hpp>
#include <exception>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

using namespace std;
// CONSTANTS
class LinearRegressionCore
{
    static double dot_product(const vector<double> &v1, const vector<double> &v2)
    {
        if (v1.size() != v2.size())
            throw runtime_error("v1 and v2 don't have same dimensions.");

        return inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
    }

    vector<double> theta;
    vector<vector<double>> x_vec;
    vector<double> y_vec;
    bool trained = false;

    vector<double> calculte_theta()
    {
        const vector<vector<double>> x = x_vec;
        vector<vector<double>> y(y_vec.size(), vector<double>());
        for (int i = 0; i < y_vec.size(); i++)
        {
            y[i].push_back(y_vec[i]);
        }
        const vector<vector<double>> xt = MatrixUtil::transpose(x);
        const vector<vector<double>> xxt_inv = MatrixUtil::inverse(MatrixUtil::product(xt, x));
        const vector<vector<double>> xty = MatrixUtil::product(xt, y);

        return MatrixUtil::transpose(MatrixUtil::product(xxt_inv, xty))[0];
    };

  public:
    LinearRegressionCore(const vector<vector<double>> &x, const vector<double> &y) : x_vec{x}, y_vec{y}
    {
        if (x.size() != y.size())
            throw runtime_error("x and y don't have same dimensions");

        if (x.size() == 0)
            throw runtime_error("cannot pass empty dataset");
        theta = calculte_theta();
        print(theta);
        this->trained = true;
    }
    LinearRegressionCore(){};

    double predict(const vector<double> &x)
    {
        if (this->trained)
        {
            return dot_product(x, theta);
        }
        else
        {
            throw runtime_error("model has not been trained");
        }
    }
    vector<double> get_theta()
    {
        if (this->trained)
        {
            return theta;
        }
        else
        {
            throw runtime_error("model has not been trained");
        }
    }
};

class LinearRegression
{
    LinearRegressionCore core;
    static vector<vector<double>> get_transformed(const vector<vector<double>> &x)
    {
        // We need to apppend all the x's by ones becuase we also need a correct theta_not (constant term)
        vector<vector<double>> x_with_constant_term = x;
        for (auto &elem : x_with_constant_term)
        {
            elem.push_back(1);
        }
        return x_with_constant_term;
    }

  public:
    LinearRegression(const vector<vector<double>> &x, const vector<double> &y)
    {
        vector<vector<double>> x_with_constant_term = get_transformed(x);
        print(x_with_constant_term);
        this->core = std::move(LinearRegressionCore{x_with_constant_term, y});
    }
    double predict(vector<double> x)
    {
        x.push_back(1);
        return core.predict(x);
    }
};