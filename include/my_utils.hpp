#include <iostream>
#include <stdexcept>
#include <vector>
#pragma once
template <typename T> void print(const std::vector<T> &v)
{
    for (auto &elem : v)
    {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}
template <typename T> void print(const std::vector<std::vector<T>> &v)
{
    for (auto &elem : v)
    {
        print(elem);
    }
    std::cout << "\n";
}

template <typename T> std::vector<T> slice(const std::vector<T> &v, const int start = -1, const int end = -1)
{
    if (start == -1 && end == -1)
    {
        return std::vector<T>(v.begin(), v.end());
    }
    else if (start == -1)
    {
        if (end < 0)
        {
            throw std::runtime_error("end must be non-negative");
        }
        return std::vector<T>(v.begin(), v.begin() + end);
    }
    else if (end == -1)
    {
        if (start < 0)
        {
            throw std::runtime_error("start must be non-negative");
        }
        return std::vector<T>(v.begin() + start, v.end());
    }
    else
    {
        if (end < 0)
        {
            throw std::runtime_error("end must be non-negative");
        }
        if (start < 0)
        {
            throw std::runtime_error("start must be non-negative");
        }
        return std::vector<T>(v.begin() + start, v.begin() + end);
    }
}
