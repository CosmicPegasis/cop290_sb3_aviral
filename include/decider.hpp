#include <stdexcept>
#include <string>
#include <vector>
#include "strategy.hpp"
using namespace std;
// Assuming we have all functions lined up
// Assuming we have respective classes lined up

class StrategyChooser{
  Strategy choice;
public:
  StrategyChooser(vector<string> args){
      if (args.size() < 1)
          throw runtime_error("argument list empty");

      if (args[0] == "BASIC")
  }
};
