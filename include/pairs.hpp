#include <iostream>
#include <strategy.hpp>

class Pairs : public Strategy
{
    std::string symbol1, symbol2, start, end;
    int x, n;
    double threshold;
    std::vector<std::string> dates;
    std::vector<std::vector<double>> data;
    void parse_args(std::vector<std::string> &args);

    void get_data();

    Statistics get_stats();

  public:
    Pairs(std::vector<std::string> args);
    ~Pairs();
};