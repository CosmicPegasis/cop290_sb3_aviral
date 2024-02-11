#include "my_utils.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

#pragma once
class MatrixUtil
{
  public:
    static std::vector<std::vector<double>> product(const std::vector<std::vector<double>> &m1,
                                                    const std::vector<std::vector<double>> &m2)
    {
        if (m2.size() == 0 || m1.size() == 0)
        {
            throw std::runtime_error("cannot compute product for zero dimension matrices");
        }
        std::vector<std::vector<double>> result(m1.size(), std::vector<double>(m2[0].size(), 0));
        for (int i = 0; i < m1.size(); i++)
        {
            for (int j = 0; j < m2.size(); j++)
            {
                for (int k = 0; k < m2[0].size(); k++)
                {
                    result[i][k] += m1[i][j] * m2[j][k];
                }
            }
        }
        return result;
    }

    static std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>> &mat)
    {
        if (mat.size() == 0)
        {
            throw std::runtime_error("cannot find inverse of matrix with no rows");
        }
        std::vector<std::vector<double>> result(mat[0].size(), std::vector<double>(mat.size()));
        for (int i = 0; i < mat.size(); i++)
        {
            for (int j = 0; j < mat[0].size(); j++)
            {
                result[j][i] = mat[i][j];
            }
        }
        return result;
    }

    static std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>> &mat)
    {
        std::vector<std::vector<double>> matc = mat;
        if (matc.size() == 0)
        {
            throw std::runtime_error("inverse of a zero dimensional matrix");
        }
        if (matc[0].size() != matc.size())
        {
            throw std::runtime_error("inverse of a non square matrix");
        }
        std::vector<std::vector<double>> result = std::vector(matc.size(), std::vector<double>(matc.size()));
        for (int i = 0; i < matc.size(); i++)
        {
            result[i][i] = 1.0;
        }

        for (int k = 0; k < matc.size(); k++)
        {
            double den = matc[k][k];
            for (int j = 0; j < matc.size(); j++)
            {
                result[k][j] /= den;
                matc[k][j] /= den;
            }
            for (int i = 0; i < matc.size(); i++)
            {
                if (i != k)
                {
                    double num = matc[i][k];
                    for (int j = 0; j < matc.size(); j++)
                    {
                        result[i][j] -= (num * result[k][j]);
                        matc[i][j] -= (num * matc[k][j]);
                    }
                }
            }
        }
        return result;
    }
};
