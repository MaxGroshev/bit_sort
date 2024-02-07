#include <algorithm>
#include "bit_sort.hpp"
#include "ui.hpp"

//-----------------------------------------------------------------------------------------

int main(int argc, char** argv) {
    std::vector<int> data = get_data<int>();
    std::vector<int> data_copy = data;

    cl_set_up_t set_up {argc, argv};
    auto start_time = time_control::chrono_cur_time ();
    prog_cl_t first_experience(std::move(set_up));
    cl::Event evt = first_experience.bit_sort(data.data(), data.size());
    auto end_time = time_control::chrono_cur_time();

    cl_ulong gpu_start, gpu_end;
    gpu_start = evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    gpu_end   = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();

    auto cpu_start = time_control::chrono_cur_time ();
    std::sort(data_copy.begin(), data_copy.end());
    auto cpu_end = time_control::chrono_cur_time ();


    utils::dump_vect(data);
    time_control::show_run_time(start_time, end_time, "GPU wall time: ");
    std::clog << "GPU pure time measured: " << (gpu_end - gpu_start) / 1000 << " ns" << std::endl;
    time_control::show_run_time(cpu_start, cpu_end, "CPU q_sort time: ");

    return 0;
}
