#include <algorithm>
#include "bit_sort.hpp"
#include "ui.hpp"

//-----------------------------------------------------------------------------------------

int main () {
    std::vector<int> data = get_data<int>();
    std::vector<int> data_copy = data;

    auto start_time = time_control::chrono_cur_time ();
    prog_cl_t first_experience("../bitonic_sort/include/bit_sort.cl");
    cl::Event Evt = first_experience.bit_sort(data.data(), data.size());
    auto end_time = time_control::chrono_cur_time();

    cl_ulong gpu_start, gpu_end;
    gpu_start = Evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    gpu_end   = Evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();

    auto cpu_start = time_control::chrono_cur_time ();
    std::sort(data_copy.begin(), data_copy.end());
    auto cpu_end = time_control::chrono_cur_time ();


    for (const auto& elem : data) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';

    time_control::show_run_time(start_time, end_time, "GPU wall time: ");
    std::cout << "GPU pure time measured: " << (gpu_end - gpu_start) / 1000000 << " ms" << std::endl;
    time_control::show_run_time(cpu_start, cpu_end, "CPU q_sort time: ");


    return 0;
}
