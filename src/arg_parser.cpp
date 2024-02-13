#include "arg_parser.hpp"
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::string> ArgParser::parse(int argc, char **argv)
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

ArgParser::ArgParser(int argc, char **argv)
{
    this->arguments = this->parse(argc, argv);
};

std::vector<std::string> ArgParser::get_arguments()
{
    return this->arguments;
}
