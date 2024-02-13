#include <fstream>
#include <string>
#include <vector>
#pragma once

class CSVWriter
{
    std::ofstream file;

  public:
    CSVWriter(std::string filename);
    void write(std::vector<std::vector<std::string>> input);
    ~CSVWriter();
};
