#ifndef MY_VECTOR_UTILS_H
#define MY_VECTOR_UTILS_H

#include <vector>
#include <iostream>

template <typename T> void print(std::vector<T> v)
{
    for (auto &elem : v)
    {
      std::cout << elem << " ";
    }
    std::cout << "\n";
}
#endif // MY_VECTOR_UTILS_H
