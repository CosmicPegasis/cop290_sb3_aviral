#include <vector>

#pragma once
class MatrixUtil
{
  public:
    static std::vector<std::vector<double>> product(const std::vector<std::vector<double>> &m1,
                                                    const std::vector<std::vector<double>> &m2);

    static std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>> &mat);

    static std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>> &mat);
};
