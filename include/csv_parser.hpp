#include <algorithm>
#include <fstream>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <string>
#include <vector>

class CSVParser
{
public:
    std::vector<std::string> parse_line(std::string line)
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
    std::vector<std::vector<std::string>> parse(std::ifstream &in_file)
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

    std::vector<std::vector<std::string>> parse_result;

public:
    CSVParser(std::string filename)
    {
        std::ifstream file;
        file.open(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("file not found");
        }

        this->parse_result = this->parse(file);
    }
    std::vector<std::string> get_column(int column_num)
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
};

class ClosePriceParser : CSVParser
{
    std::vector<double> parse_close_price(std::vector<std::string> unparsed_close_price)
    {
        std::vector<double> result(unparsed_close_price.size());
        std::transform(unparsed_close_price.begin(), unparsed_close_price.end(), result.begin(),
                       [&](std::string elem) { return std::stod(elem); });
        return result;
    }
    std::vector<double> close_price;
    std::vector<std::string> dates;

public:
    ClosePriceParser(std::string filename) : CSVParser{filename}
    {
        this->dates = this->get_column(0);
        this->close_price = parse_close_price(this->get_column(1));
    }

    std::vector<double> get_close_price()
    {
        return this->close_price;
    }
    std::vector<std::string> get_dates()
    {
        return this->dates;
    }
};

// #include <algorithm>
// #include <fstream>
// #include <iostream>
// #include <locale>
// #include <stdexcept>
// #include <string>
// #include <vector>
// using namespace std;
// class CSVParser
// {
//   public:
//     vector<string> parse_line(string line)
//     {
//         vector<string> result;
//         string current_word = "";
//         for (int i = 0; i < line.size(); i++)
//         {
//             if (line[i] == ',')
//             {
//                 result.push_back(current_word);
//                 current_word = "";
//             }
//             else
//             {
//                 current_word += line[i];
//             }
//         }
//         if (current_word.size() > 0)
//         {
//             result.push_back(current_word);
//         }
//         return result;
//     };
//     vector<vector<string>> parse(ifstream &in_file)
//     {
//         vector<vector<string>> result;
//         while (!in_file.eof())
//         {
//             string line;
//             in_file >> line;
//             vector<string> tokenised_line = parse_line(line);
//             while (tokenised_line.size() > result.size())
//             {
//                 result.push_back(vector<string>());
//             }
//             for (int i = 0; i < tokenised_line.size(); i++)
//             {
//                 result[i].push_back(tokenised_line[i]);
//             }
//         }
//         return result;
//     }

//     vector<vector<string>> parse_result;

//   public:
//     CSVParser(string filename)
//     {
//         ifstream file;
//         file.open(filename);
//         if (!file.is_open())
//         {
//             throw runtime_error("file not found");
//         }

//         this->parse_result = this->parse(file);
//     }
//     vector<string> get_column(int column_num)
//     {
//         // zero indexed
//         if (column_num >= parse_result.size())
//         {
//             throw runtime_error("column doesn't exist");
//         }
//         else
//         {
//             return parse_result[column_num];
//         }
//     }
// };
// class ClosePriceParser : CSVParser
// {
//     vector<double> parse_close_price(vector<string> unparsed_close_price)
//     {
//         vector<double> result(unparsed_close_price.size());
//         transform(unparsed_close_price.begin(), unparsed_close_price.end(), result.begin(),
//                   [&](string elem) { return stod(elem); });
//         return result;
//     }
//     vector<double> close_price;
//     vector<string> dates;

//   public:
//     ClosePriceParser(string filename) : CSVParser{filename}
//     {
//         this->dates = this->get_column(0);
//         this->close_price = parse_close_price(this->get_column(1));
//     }

//     vector<double> get_close_price()
//     {
//         return this->close_price;
//     }
//     vector<string> get_dates()
//     {
//         return this->dates;
//     }
// };
