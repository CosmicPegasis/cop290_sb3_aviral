#include <iostream>
#include <stdexcept>
#include <vector>

#define double long double
using namespace std;

void print(vector<double> v)
{
    cout.precision(2);
    for (auto &elem : v)
    {
        cout << fixed << elem << " ";
    }
    cout << "\n";
}
void print(vector<vector<double>> v)
{
    for (auto &elem : v)
    {
        print(elem);
    }
}
class MatrixUtil
{
  public:
    static vector<vector<double>> matrix_product(const vector<vector<double>> &m1, const vector<vector<double>> &m2)
    {
        if (m2.size() == 0 || m1.size() == 0)
        {
            throw runtime_error("cannot compute product for zero dimension matrices");
        }
        vector<vector<double>> result(m1.size(), vector<double>(m2[0].size(), 0));
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

    static vector<vector<double>> transpose(const vector<vector<double>> &mat)
    {
        if (mat.size() == 0)
        {
            throw runtime_error("cannot find inverse of matrix with no rows");
        }
        vector<vector<double>> result(mat[0].size(), vector<double>(mat.size()));
        for (int i = 0; i < mat.size(); i++)
        {
            for (int j = 0; j < mat[0].size(); j++)
            {
                result[j][i] = mat[i][j];
            }
        }
        return result;
    }
    static vector<vector<double>> cofactor(const vector<vector<double>> &mat, int n, int m)
    {
        if (mat.size() == 0)
        {
            throw runtime_error("cofactor of zero dimensional matrix");
        }
        if (mat.size() == 1)
        {
            throw runtime_error("cofactor of a 1x1 matrix");
        }
        if (mat.size() != mat[0].size())
        {
            throw runtime_error("cofactor of non square matrix");
        }
        if (m >= mat.size() || n >= mat.size())
        {
            throw runtime_error("cofactor indices out of bounds");
        }
        vector<vector<double>> result(mat.size() - 1, vector<double>(mat.size() - 1, 0));
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
    static double determinant(const vector<vector<double>> &mat)
    {
        if (mat.size() == 0)
        {
            throw runtime_error("cannot find determinant of a zero dimension matrix");
        }
        if (mat.size() != mat[0].size())
        {
            throw runtime_error("cannot find determinant of a non square matrix");
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

    static vector<vector<double>> inverse(const vector<vector<double>> &mat)
    {
        if (mat.size() == 0)
        {
            throw runtime_error("inverse of a zero dimensional matrix");
        }
        if (mat[0].size() != mat.size())
        {
            throw runtime_error("inverse of a non square matrix");
        }
        vector<vector<double>> result(mat.size(), vector<double>(mat.size(), 0));
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
