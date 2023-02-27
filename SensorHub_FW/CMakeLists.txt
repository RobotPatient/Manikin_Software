# Works with 3.11 and tested through 3.23
cmake_minimum_required(VERSION 3.11)

# Project name and a few useful settings. Other commands can pick up the results
project(
        Manikin_SensorHub_FW
        VERSION 1.0
        DESCRIPTION "Fw for the SensorHub board of the Manikin"
        LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)

endif()

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
        AND BUILD_TESTING)
    add_subdirectory(test)
endif()


#include(CMakeListsPIO.txt)

#add_custom_target(
#    Production ALL
#    COMMAND pio run
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

