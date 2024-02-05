#include <algorithm>
// #include <stdexcept>
#include "bit_sort.hpp"
#include "ui.hpp"

//-----------------------------------------------------------------------------------------

int main () {
    // std::vector<int> data = get_data<int>();
    // for (const auto& elem : data)
    //     std::cout << elem << ' ';

    prog_cl_t first_experience("../bitonic_sort/include/bit_sort.cl");
    first_experience.bit_sort(3);

    return 0;
}
