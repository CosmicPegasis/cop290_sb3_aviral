// #include "linear_regression.cpp"
#include <csv_writer.hpp>
#include <decider.hpp>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    // Write a wrapper for LinearRegression
    // Write a wrapper for all of Daksh's code
    // vector<vector<double>> x = {
    //     {0.509, 0.508, 0.011, 0.551, 0.671, 0.632, 0.202}, {0.728, 0.637, 0.473, 0.783, 0.472, 0.394, 0.968},
    //     {0.935, 0.644, 0.901, 0.312, 0.055, 0.496, 0.364}, {0.027, 0.89, 0.876, 0.537, 0.157, 0.866, 0.225},
    //     {0.698, 0.31, 0.095, 0.969, 0.274, 0.646, 0.79},   {0.052, 0.813, 0.676, 0.111, 0.277, 0.117, 0.058},
    //     {0.824, 0.83, 0.199, 0.75, 0.659, 0.174, 0.657},   {0.827, 0.556, 0.905, 0.903, 0.789, 0.627, 0.457},
    //     {0.884, 0.472, 0.381, 0.437, 0.961, 0.76, 0.806},  {0.043, 0.278, 0.139, 0.478, 0.124, 0.972, 0.745}};
    // vector<double> y = {3.084, 4.455, 3.708, 3.578, 3.78, 2.103, 4.093, 5.064, 4.701, 2.779};
    vector<string> args = {"LINEAR_REGRESSION", "SBIN",       "5",          "20",
                           "03/02/2020",        "04/02/2021", "05/02/2023", "09/02/2023"};
    StrategyChooser choose(args);
    CSVWriter order_statistics("order_statistics.csv");
    CSVWriter daily_cashflow("daily_cashflow.csv");
    order_statistics.write(choose.get_orders());
    daily_cashflow.write(choose.get_cashflow());
    return 0;
}

// Strategy Decider
/*
void basic(std::vector<std::vector<double>> data, int n, int x, std::vector<std::string> date);

void dma(vector<vector<double>> data, int n, int x, vector<string> date, int p);

void dma(vector<vector<double>> data, int n, int x, vector<string> date, double p,int max_hold_days,double c1,double c2)

void macd(vector<double> close_price, vector<double> open_price, vector<string> date, int x)

void rsi(vector<vector<double>> data, int n, int x, vector<string> date, double oversold, double overbought)


void BestOfALL // TODO

void pairs_sl(vector<vector<double>> data, int n, int x, vector<string> date, int p, double threshold,double
stop_loss_threshold)
 */
