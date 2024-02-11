#include <stdexcept>
#include <string>
#include <vector>

#pragma once
class ArgParser
{
  private:
    std::vector<std::string> arguments;

    std::vector<std::string> parse(int argc, char **argv)
    {
        if (argc < 1)
        {
            throw std::runtime_error("invalid number of arguments");
        }
        std::vector<std::string> args(argc - 1);
        for (int i = 1; i < argc; i++)
            args[i - 1] = argv[i];
        return args;
    }

  public:
    ArgParser(int argc, char **argv)
    {
        this->arguments = this->parse(argc, argv);
    };

    std::vector<std::string> get_arguments()
    {
        return this->arguments;
    }
};
