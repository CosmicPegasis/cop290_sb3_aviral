#include "csv_writer.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

CSVWriter::CSVWriter(std::string filename)
{
    file.open(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening file");
    }
}
void CSVWriter::write(std::vector<std::vector<std::string>> input)
{
    for (std::vector<std::string> &line : input)
    {
        int word_count = 0;
        for (std::string &word : line)
        {
            file << word;
            word_count++;
            if (word_count != line.size())
            {
                file << ',';
            }
        }
        file << "\n";
    }
}
CSVWriter::~CSVWriter()
{
    file.close();
}
