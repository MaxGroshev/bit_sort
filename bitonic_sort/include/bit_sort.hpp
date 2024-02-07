#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "time_control.hpp"
#include <CL/opencl.hpp>
#include "utils.hpp"
#include "cl_set_up.hpp"

//-----------------------------------------------------------------------------------------

class prog_cl_t {
    private:
        cl::Platform plat_;
        cl::Context  context_;
        cl::CommandQueue com_q_;

        std::string kernel_ = "Hello world";
        std::string func_name = "comp_and_swap";

        inline cl::Platform get_platform();
        inline cl::Context  get_context(cl_platform_id plat_id);

        cl_set_up_t set_up_;

        using bit_sort_t = cl::KernelFunctor<cl::Buffer, int, int>;
    public:

        prog_cl_t(cl_set_up_t set_up) :
            set_up_(set_up),
            plat_(get_platform()),
            context_(get_context(plat_())),
            com_q_(context_, cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder),
            kernel_(get_kernel_from_file(set_up.kernel_dir))
            {
                cl::string name    = plat_.getInfo<CL_PLATFORM_NAME>();
                cl::string profile = plat_.getInfo<CL_PLATFORM_PROFILE>();
                std::clog << "\nSelected: " << name << ": " << profile << std::endl;
            }

        inline std::string get_kernel_from_file(const char* ker_dir);

        inline cl::Event bit_sort(int* data, int data_size);
};

//-----------------------------------------------------------------------------------------

cl::Event prog_cl_t::bit_sort(int* data, int data_size) {

    cl::Event evt = {};
    try {
        int data_buf_size = (data_size) * sizeof(int);
        cl::Buffer cl_data(context_, CL_MEM_READ_WRITE, data_buf_size);
        cl::copy(com_q_, data, data + data_size, cl_data);

        cl::Program program(context_, kernel_, true);
        bit_sort_t prog(program, func_name);

        int loc_sz = set_up_.local_range_sz;
        if (data_size < loc_sz)
            loc_sz = 1;

        cl::NDRange local_range(loc_sz);
        cl::NDRange global_range(data_size);
        cl::EnqueueArgs Args(com_q_, global_range, local_range);

        for (int i = 2; i <= data_size; i *= 2) {
            for (int j = i / 2; j > 0; j /= 2) {
                evt = prog(Args, cl_data, i, j);
                evt.wait();
            }
        }
        cl::copy(com_q_, cl_data, data, data + data_size);
    } catch (cl::BuildError &err) {
        std::cerr << "cl build err: " << err.err() << ":" << err.what() << std::endl;
        for (auto e : err.getBuildLog())
            std::cerr << e.second;
        return evt;
    } catch (cl::Error &err) {
        std::cerr << "cl run error: " << err.err() << ":" << err.what() << std::endl;
        return evt;
    } catch (...) {
        throw;
    };

    return evt;
}

//-----------------------------------------------------------------------------------------

std::string prog_cl_t::get_kernel_from_file(const char* ker_dir) {
    std::string ker_str;
    std::ifstream in_file;
    std::stringstream in_str;

    in_file.open(ker_dir);
    if (!in_file.is_open())
        throw("Wrong direction of file");
    in_str << in_file.rdbuf();
    in_file.close();

    ker_str = in_str.str();
    return ker_str;
}

cl::Platform prog_cl_t::get_platform() {
    cl::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for (auto plt : platforms) {
        cl_uint n_of_devices = 0;
        //CL_DEVICE_TYPE_GPU == 4, CL_DEVICE_TYPE_CPU == 2
        clGetDeviceIDs(plt(), CL_DEVICE_TYPE_GPU, 0, NULL, &n_of_devices);//from global namespace as C API
        // std::cout << n_of_devices << '\n';
        if (n_of_devices > 0) {
            return cl::Platform(plt);
        }
    }
    throw std::runtime_error("No platform selected");

}

cl::Context prog_cl_t::get_context(cl_platform_id plat_id) {
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(plat_id), 0
    };

    return cl::Context(CL_DEVICE_TYPE_GPU, properties);
}
