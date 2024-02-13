#include "csv_parser.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::string> CSVParser::parse_line(std::string line)
{
    std::vector<std::string> result;
    std::string current_word = "";
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == ',')
        {
            result.push_back(current_word);
            current_word = "";
        }
        else
        {
            current_word += line[i];
        }
    }
    if (current_word.size() > 0)
    {
        result.push_back(current_word);
    }
    return result;
};
std::vector<std::vector<std::string>> CSVParser::parse(std::ifstream &in_file)
{
    std::vector<std::vector<std::string>> result;
    while (!in_file.eof())
    {
        std::string line;
        in_file >> line;
        std::vector<std::string> tokenised_line = parse_line(line);
        while (tokenised_line.size() > result.size())
        {
            result.push_back(std::vector<std::string>());
        }
        for (int i = 0; i < tokenised_line.size(); i++)
        {
            result[i].push_back(tokenised_line[i]);
        }
    }
    return result;
}

CSVParser::CSVParser(std::string filename)
{
    std::ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("file not found");
    }

    this->parse_result = this->parse(file);
    file.close();
}
std::vector<std::string> CSVParser::get_column(int column_num)
{
    // zero indexed
    if (column_num >= parse_result.size())
    {
        throw std::runtime_error("column doesn't exist");
    }
    else
    {
        return parse_result[column_num];
    }
}

std::vector<double> ClosePriceParser::parse_close_price(std::vector<std::string> unparsed_close_price)
{
    std::vector<double> result(unparsed_close_price.size());
    std::transform(unparsed_close_price.begin(), unparsed_close_price.end(), result.begin(),
                   [&](std::string elem) { return std::stod(elem); });
    return result;
}
std::vector<double> close_price;
std::vector<std::string> dates;

ClosePriceParser::ClosePriceParser(std::string filename) : CSVParser{filename}
{
    this->dates = this->get_column(0);
    this->close_price = parse_close_price(this->get_column(1));
}

std::vector<double> ClosePriceParser::get_close_price()
{
    return this->close_price;
}
std::vector<std::string> ClosePriceParser::get_dates()
{
    return this->dates;
}
