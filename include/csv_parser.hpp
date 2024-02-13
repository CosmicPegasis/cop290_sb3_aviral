#include <string>
#include <vector>

#pragma once
class CSVParser
{
  public:
    std::vector<std::string> parse_line(std::string line);
    std::vector<std::vector<std::string>> parse(std::ifstream &in_file);

    std::vector<std::vector<std::string>> parse_result;

  public:
    CSVParser(std::string filename);
    std::vector<std::string> get_column(int column_num);
};

class ClosePriceParser : CSVParser
{
    std::vector<double> parse_close_price(std::vector<std::string> unparsed_close_price);
    std::vector<double> close_price;
    std::vector<std::string> dates;

  public:
    ClosePriceParser(std::string filename);

    std::vector<double> get_close_price();
    std::vector<std::string> get_dates();
};
