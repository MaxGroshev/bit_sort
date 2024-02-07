cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(bit_sort)

#-----------------------------------------------------------------------------------------

set(TESTING_ENABLED ON CACHE BOOL [FORCE])

#-----------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(cmake/build_flags.cmake)
    message("Debug_version")
endif ()

#-----------------------------------------------------------------------------------------


# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(./bitonic_sort/)
add_subdirectory(debug_utils/)

# if (TESTING_ENABLED)
#     add_subdirectory(./tests/)
# endif()

unset(TESTING_ENABLED CACHE)
unset(RUN_SET CACHE)


