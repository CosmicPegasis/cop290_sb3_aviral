#include <string>
#include <vector>

enum StrategyType
{
    BasicStrat,
    DMAStrat,
    DMAPlusPlusStrat,
    MACDStrat,
    RSIStrat,
    ADXStrat,
    LinearRegressionStrat,
    BestOfAllStrat,
    PairsStrat,
};
class Strategy
{
    // double get_final_pnl();
    // std::vector<double> get_daily_cashflow();
    // std::vector<OrderStatistic> get_order_statistics();
};

class Basic : public Strategy
{
  public:
    Basic(std::vector<std::string> args){};
};

class DMA : public Strategy
{
  public:
    DMA(std::vector<std::string> args){};
};

class DMAPlusPlus : public Strategy
{
  public:
    DMAPlusPlus(std::vector<std::string> args){};
};

class MACD : public Strategy
{
  public:
    MACD(std::vector<std::string> args){};
};

class RSI : public Strategy
{
  public:
    RSI(std::vector<std::string> args){};
};

class ADX : public Strategy
{
  public:
    ADX(std::vector<std::string> args){};
};
class LinearRegression : public Strategy
{
  public:
    LinearRegression(std::vector<std::string> args){};
};

class BestOfAll : public Strategy
{
  public:
    BestOfAll(std::vector<std::string> args){};
};

class PairsSL : public Strategy
{
  public:
    PairsSL(std::vector<std::string> args){};
};
