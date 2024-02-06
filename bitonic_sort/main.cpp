#include <algorithm>
// #include <stdexcept>
#include "bit_sort.hpp"
#include "ui.hpp"

//-----------------------------------------------------------------------------------------

int main () {
    std::vector<int> data = get_data<int>();
    for (const auto& elem : data)
        std::cout << elem;

    try {
        prog_cl_t first_experience("../bitonic_sort/include/bit_sort.cl");
        first_experience.bit_sort(data.data(), data.size());
    } catch (cl::BuildError &err) {
        std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
                << std::endl;
        std::cerr << "-- Log --\n";
        for (auto e : err.getBuildLog())
        std::cerr << e.second;
        std::cerr << "-- End log --\n";
        throw;
    } catch (cl::Error &err) {
        std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
        throw;
    }

    return 0;
}
