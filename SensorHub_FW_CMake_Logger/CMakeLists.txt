cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
  SAMD_Toolchain
  GIT_REPOSITORY https://github.com/Hoog-V/SAMD_Toolchain.git
  GIT_TAG        main
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/SDK
)

FetchContent_MakeAvailable(SAMD_Toolchain)

FetchContent_Declare (
  Universal_hal
  GIT_REPOSITORY https://github.com/Hoog-V/Universal_hal.git
  GIT_TAG a2c65b237737468c8b5ce4b2f5704e4eaf97713d
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/Universal_hal
)


FetchContent_Declare(
  FreeRTOS
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel
  GIT_TAG        V11.0.1
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS
)

FetchContent_Declare(
  Manikin_Software_Libraries_V2
  GIT_REPOSITORY https://github.com/RobotPatient/Manikin_Software_Libraries_V2
  GIT_TAG topic-usb-service-protocol
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/Manikin_Software_Libraries_V2
)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/SDK/samd21g18_toolchain_armgcc.cmake)

project(SensorHub_FW_Logger C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(UHAL_BUILD_TESTS OFF)
set(UHAL_ENABLE_USB_SERIAL_MODULE ON)

add_compile_options(-fno-exceptions
                    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
                    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
                    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>)



FetchContent_MakeAvailable(Universal_hal)

# Select the heap port.  values between 1-4 will pick a heap.
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

# Select the native compile PORT
set(FREERTOS_PORT "GCC_ARM_CM0" CACHE STRING "" FORCE)

add_library(freertos_config INTERFACE)

FetchContent_MakeAvailable(FreeRTOS)

add_library(FreeRTOS ALIAS freertos_kernel)

target_include_directories(freertos_config INTERFACE "config/")

FetchContent_MakeAvailable(Manikin_Software_Libraries_V2)

add_executable(SensorHub_FW_CMake_Logger.elf "src/main.cpp" "src/FreeRTOS_hooks.c" "src/libc_hooks.c" ${STARTUP_SCRIPT_SOURCES})
target_link_libraries(SensorHub_FW_CMake_Logger.elf Universal_hal Universal_hal_usb freertos_kernel usb_service_protocol sensor_compression fram_driver)


target_sources(Universal_hal_usb_conf INTERFACE "src/usb_descriptors.c")
target_include_directories(Universal_hal_usb_conf INTERFACE config/)
target_link_libraries(Universal_hal_usb_conf INTERFACE freertos_kernel)