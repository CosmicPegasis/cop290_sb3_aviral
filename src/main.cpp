#include "arg_parser.hpp"
#include "my_utils.hpp"
#include "../daksh/basic.cpp"
#include "csv_parser.hpp"
int main(int argc, char** argv)
{
  // Write an argument parser
  // Write a choice decider
  // Glue together code to check if it works
  // Write a makefile
  ArgParser args(argc, argv);
  ClosePriceParser csv("close_price.csv");
  vector<double> close_price = csv.get_close_price();
  vector<vector<double>> data(close_price.size(), vector<double>(2,0));
  for(int i = 0; i < close_price.size(); i++){
    data[i][0] = close_price[i];
  }
  print(close_price);
  print(csv.get_dates());
  vector<string> date = csv.get_dates();
  int x = 1;
  int n = 2;
  basic(data, n, x, date);
return 0;
}

// Strategy Decider
/*
void basic(std::vector<std::vector<double>> data, int n, int x, std::vector<std::string> date);

void dma(vector<vector<double>> data, int n, int x, vector<string> date, int p);

void dma(vector<vector<double>> data, int n, int x, vector<string> date, double p,int max_hold_days,double c1,double c2)

void macd(vector<double> close_price, vector<double> open_price, vector<string> date, int x)

void rsi(vector<vector<double>> data, int n, int x, vector<string> date, double oversold, double overbought)

void LinearRegression // TODO

void BestOfALL // TODO

void pairs_sl(vector<vector<double>> data, int n, int x, vector<string> date, int p, double threshold,double stop_loss_threshold)
 */
