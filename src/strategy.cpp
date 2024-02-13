#include "strategy.hpp"
double Strategy::get_final_pnl()
{
    return stats.final_pnl;
}
std::vector<std::pair<std::string, double>> Strategy::get_daily_cashflow()
{
    return stats.daily_cashflow;
};
std::vector<std::vector<std::string>> Strategy::get_order_statistics()
{
    return stats.order_statistics;
};
std::vector<std::vector<std::string>> Strategy::get_order_statistics2()
{
    return stats.order_statistics2;
};