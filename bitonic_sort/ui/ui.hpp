#pragma once

#include <iostream>
#include <vector>

//-----------------------------------------------------------------------------------------

template<typename T>
std::vector<T> get_data (std::istream & in_strm = std::cin) {
    if (in_strm.eof ())
        return std::vector<T>{};

    int n_elems = 0;
    in_strm >> n_elems;

    std::vector<T> data;
    T elem{};
    for (int i = 0; i < n_elems; i++) { //^D to exit from cin input
        in_strm >> elem;
        data.push_back(elem);
    }
    return data;
}
