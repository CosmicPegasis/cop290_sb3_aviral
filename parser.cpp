#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

enum Strategy
{
    BASIC,
    DMA,
    DMAPlusPlus,
    MACD,
    RSI,
    ADX,
    LinearRegression,
    BestOfAll,
    PAIRS,
};
template <typename T> void print(vector<T> v)
{
    for (auto &elem : v)
    {
        cout << elem << " ";
    }
    cout << "\n";
}
class CSVParser
{
  public:
    vector<string> parse_line(string line)
    {
        vector<string> result;
        string current_word = "";
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
    vector<vector<string>> parse(ifstream &in_file)
    {
        vector<vector<string>> result;
        while (!in_file.eof())
        {
            string line;
            in_file >> line;
            vector<string> tokenised_line = parse_line(line);
            while (tokenised_line.size() > result.size())
            {
                result.push_back(vector<string>());
            }
            for (int i = 0; i < tokenised_line.size(); i++)
            {
                result[i].push_back(tokenised_line[i]);
            }
        }
        return result;
    }

    vector<vector<string>> parse_result;

  public:
    CSVParser(string filename)
    {
        ifstream file;
        file.open(filename);
        if (!file.is_open())
        {
            throw runtime_error("file not found");
        }

        this->parse_result = this->parse(file);
    }
    vector<string> get_column(int column_num)
    {
        // zero indexed
        if (column_num >= parse_result.size())
        {
            throw runtime_error("column doesn't exist");
        }
        else
        {
            return parse_result[column_num];
        }
    }
};
class ClosePriceParser : CSVParser
{
    vector<double> parse_close_price(vector<string> unparsed_close_price)
    {
        vector<double> result(unparsed_close_price.size());
        transform(unparsed_close_price.begin(), unparsed_close_price.end(), result.begin(),
                  [&](string elem) { return stod(elem); });
        return result;
    }
    vector<double> close_price;
    vector<string> dates;

  public:
    ClosePriceParser(string filename) : CSVParser{filename}
    {
        this->dates = this->get_column(0);
        this->close_price = parse_close_price(this->get_column(1));
    }

    vector<double> get_close_price()
    {
        return this->close_price;
    }
    vector<string> get_dates()
    {
        return this->dates;
    }
};
int main()
{
    ClosePriceParser csv("close_price.csv");
    print(csv.get_close_price());
    print(csv.get_dates());
    return 0;
}
