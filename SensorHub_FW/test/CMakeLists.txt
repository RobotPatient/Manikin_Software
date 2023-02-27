cmake_minimum_required(VERSION 3.8)

enable_testing()


if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(googletest)
add_subdirectory(i2c_tests)