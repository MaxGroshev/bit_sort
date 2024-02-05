#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#define CL_HPP_ENABLE_EXCEPTIONS


#include <CL/opencl.hpp>

//-----------------------------------------------------------------------------------------
//set cmake ocl req_ver
class prog_cl_t {
    private:
        cl::Platform plat_;
        cl::Context  context_;
        cl::CommandQueue com_q_;

        std::string kernel_ = "Hello world";

        inline cl::Platform get_platform();
        inline cl::Context  get_context(cl_platform_id plat_id);

        using bit_sort_t = cl::KernelFunctor<int>;
    public:

        prog_cl_t(const char* ker_dir) :
            plat_(get_platform()),
            context_(get_context(plat_())),
            com_q_(context_, cl::QueueProperties::OutOfOrder), //or cl::QueueProperties::Profiling
            kernel_(get_kernel_from_file(ker_dir))
            {
                cl::string name    = plat_.getInfo<CL_PLATFORM_NAME>();
                cl::string profile = plat_.getInfo<CL_PLATFORM_PROFILE>();
                std::cout << "Selected: " << name << ": " << profile << std::endl;
            }

        inline std::string get_kernel_from_file(const char* ker_dir);

        cl::Event bit_sort(int count);
};

//-----------------------------------------------------------------------------------------

cl::Event prog_cl_t::bit_sort(int count) {
    try {
        cl::Program program(context_, kernel_, true);

        std::cout << kernel_ << '\n';
        bit_sort_t prog(program, "hello_world");

        cl::NDRange global_range(count);
        // cl::NDRange local_range(0, 1);
        cl::EnqueueArgs Args(com_q_, global_range);
        cl::Event Evt = prog(Args, 10);
        std::cout << "Here\n";
        Evt.wait();
        return Evt;
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
