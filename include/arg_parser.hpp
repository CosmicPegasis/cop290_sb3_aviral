#include <stdexcept>
#include <string>
#include <vector>

#pragma once
class ArgParser
{
  private:
    std::vector<std::string> arguments;

    std::vector<std::string> parse(int argc, char **argv);

  public:
    ArgParser(int argc, char **argv);

    std::vector<std::string> get_arguments();
};
