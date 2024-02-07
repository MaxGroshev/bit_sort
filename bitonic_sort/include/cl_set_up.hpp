#include <iostream>
#include <stdexcept>

//-----------------------------------------------------------------------------------------

struct cl_set_up_t {
    public:
        int local_range_sz = 64;
        const char* kernel_dir = "../bitonic_sort/include/bit_sort.cl";

        cl_set_up_t();
        cl_set_up_t (const int argc, char** argv) {
            for (int i = 0; i < argc; i++) {
                std::string_view arg = argv[i];
                auto arg_end = arg.data() + arg.size();
                if (arg.starts_with("-lrs=")) {
                    auto res_flag =  std::from_chars(arg.data() + strlen("-lrs="), arg_end, local_range_sz);
                    if (res_flag.ec == std::errc::invalid_argument)
                        throw std::runtime_error(argv[i]);
                }
            }
        }
};
