set(SRCS
        bit_sort.cpp
        utils.cpp
        cl_set_up.cpp
)


add_library(src ${SRCS})
target_include_directories(src PRIVATE ${CMAKE_BINARY_DIR})

