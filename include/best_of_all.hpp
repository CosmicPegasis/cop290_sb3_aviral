#include "strategy.hpp"
#include <string>

#pragma once
class BestOfAll : public Strategy
{
    const std::string basic_n = "7", dma_n = "50", dmapp_n = "14", x = "5", dma_p = "2", dmapp_p = "5",
                      dmapp_max_hold_days = "28", adx_threshold = "25", rsi_oversold = "30", rsi_overbought = "70",
                      dmapp_c1 = "2", dmapp_c2 = "0.2", lr_p = "2";
    std::string symbol, start, end;
    std::vector<std::string> dates;
    std::vector<Strategy> strategies;
    void parse_args(std::vector<std::string> &args);

    void get_data();
    Statistics get_stats();

  public:
    BestOfAll(std::vector<std::string> args);
    ~BestOfAll();
};