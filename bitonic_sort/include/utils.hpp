#pragma once
#include <vector>
#include <iostream>

//-----------------------------------------------------------------------------------------

namespace utils {

template<typename T>
void dump_vect(const std::vector<T>& vect) {
    for (const auto& elem : vect)
        std::cout << elem << " ";
    std::cout << '\n';
}

}

