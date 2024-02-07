#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "time_control.hpp"
#include <CL/opencl.hpp>

//-----------------------------------------------------------------------------------------

class prog_cl_t {
    private:
        cl::Platform plat_;
        cl::Context  context_;
        cl::CommandQueue com_q_;

        std::string kernel_ = "Hello world";

        inline cl::Platform get_platform();
        inline cl::Context  get_context(cl_platform_id plat_id);

        inline cl::Event bit_split();
        inline cl::Event bit_merge();

        using bit_sort_t = cl::KernelFunctor<cl::Buffer, int, int>;
    public:

        prog_cl_t(const char* ker_dir) :
            plat_(get_platform()),
            context_(get_context(plat_())),
            com_q_(context_, cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder),
            kernel_(get_kernel_from_file(ker_dir))
            {
                cl::string name    = plat_.getInfo<CL_PLATFORM_NAME>();
                cl::string profile = plat_.getInfo<CL_PLATFORM_PROFILE>();
                std::cout << "Selected: " << name << ": " << profile << std::endl;
            }

        inline std::string get_kernel_from_file(const char* ker_dir);

        inline cl::Event bit_sort(int* data, int data_size);
};

//-----------------------------------------------------------------------------------------

cl::Event prog_cl_t::bit_sort(int* data, int data_size) {

    int data_buf_size = (data_size) * sizeof(int);
    cl::Buffer cl_data(context_, CL_MEM_READ_WRITE, data_buf_size);
    cl::copy(com_q_, data, data + data_size, cl_data);

    cl::Program program(context_, kernel_, true);
    bit_sort_t prog(program, "comp_and_swap");

    // cl::NDRange local_range(2);
    cl::NDRange global_range(data_size);
    cl::EnqueueArgs Args(com_q_, global_range);

    cl::Event Evt = {};
    for (int i = 2; i <= data_size; i *= 2) {
        for (int j = i / 2; j > 0; j /= 2) {
            Evt = prog(Args, cl_data, i, j);
            Evt.wait();
            cl::copy(com_q_, cl_data, data, data + data_size);
        }
    }
    // for (int i = 0; i < data_size; i++) {
    //     std::cout << data[i] << ' ';
    // }
    // std::cout << '\n';
    // Evt.wait();

    return Evt;
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
