#include <iostream>
#include <stdexcept>
#include <vector>

class MatrixUtil
{
  public:
    static std::vector<std::vector<double>> matrix_product(const std::vector<std::vector<double>> &m1,
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
    static std::vector<std::vector<double>> cofactor(const std::vector<std::vector<double>> &mat, int n, int m)
    {
        if (mat.size() == 0)
        {
            throw std::runtime_error("cofactor of zero dimensional matrix");
        }
        if (mat.size() == 1)
        {
            throw std::runtime_error("cofactor of a 1x1 matrix");
        }
        if (mat.size() != mat[0].size())
        {
            throw std::runtime_error("cofactor of non square matrix");
        }
        if (m >= mat.size() || n >= mat.size())
        {
            throw std::runtime_error("cofactor indices out of bounds");
        }
        std::vector<std::vector<double>> result(mat.size() - 1, std::vector<double>(mat.size() - 1, 0));
        for (int i = n + 1; i < mat.size(); i++)
        {
            for (int j = m + 1; j < mat.size(); j++)
            {
                result[i - 1][j - 1] = mat[i][j];
            }
            for (int j = 0; j < m; j++)
            {
                result[i - 1][j] = mat[i][j];
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                result[i][j] = mat[i][j];
            }
            for (int j = m + 1; j < mat.size(); j++)
            {
                result[i][j - 1] = mat[i][j];
            }
        }
        return result;
    }
    static double determinant(const std::vector<std::vector<double>> &mat)
    {
        if (mat.size() == 0)
        {
            throw std::runtime_error("cannot find determinant of a zero dimension matrix");
        }
        if (mat.size() != mat[0].size())
        {
            throw std::runtime_error("cannot find determinant of a non square matrix");
        }
        if (mat.size() == 1)
        {
            return mat[0][0];
        }
        double result = 0;
        int sign = 1;
        for (int k = 0; k < mat[0].size(); k++)
        {
            result += sign * mat[0][k] * determinant(cofactor(mat, 0, k));
            sign *= -1;
        }
        return result;
    };

    static std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>> &mat)
    {
        if (mat.size() == 0)
        {
            throw std::runtime_error("inverse of a zero dimensional matrix");
        }
        if (mat[0].size() != mat.size())
        {
            throw std::runtime_error("inverse of a non square matrix");
        }
        std::vector<std::vector<double>> result(mat.size(), std::vector<double>(mat.size(), 0));
        double det = determinant(mat);
        for (int i = 0; i < mat.size(); i++)
        {
            for (int j = 0; j < mat[0].size(); j++)
            {
                int sign = 1;
                if ((i + j) % 2)
                    sign = -1;
                result[i][j] = sign * determinant(cofactor(mat, i, j)) / det;
            }
        }
        return transpose(result);
    }
};
