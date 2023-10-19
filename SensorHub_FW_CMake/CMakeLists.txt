cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(ATMELSAMD21 C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(MCPU cortex-m0plus)

add_compile_options(-mcpu=${MCPU} -mthumb)
add_compile_options(-mlong-calls -g3 -Wall -c
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>)


if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()


include_directories(
        ${CMAKE_SOURCE_DIR}/board_sdk/CMSIS_5/CMSIS/Core/Include/
        ${CMAKE_SOURCE_DIR}/board_sdk/cmsis-header-sam/samd21a/include/
        ${CMAKE_SOURCE_DIR}/Universal_hall/hal_gpio/
        ${CMAKE_SOURCE_DIR}/Universal_hall/hal_gpio/platform/atmelsam/
        ${CMAKE_SOURCE_DIR}/Universal_hall/hal_i2c/
        ${CMAKE_SOURCE_DIR}/Universal_hall/hal_i2c/platform/atmelsam/
        ${CMAKE_SOURCE_DIR}/Universal_hall/platform_dep/atmelsam/
        ${CMAKE_SOURCE_DIR}/src
        .
)

add_definitions(-D__SAMD21G18A__ -ffunction-sections -fdata-sections -fno-exceptions -fno-strict-aliasing -fno-common -fmessage-length=0 -fstack-usage -Dprintf=iprintf)

set(FREERTOS_PORT GCC_ARM_CM0 CACHE STRING "")

file(GLOB_RECURSE SOURCES
        "board_sdk/gcc/*.*"
        "board_sdk/gcc/gcc/*.*"
        "src/port.c"
        "src/main.cpp"
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/board_sdk/gcc/gcc/samd21g18a_flash.ld)

add_link_options(-Wl,--start-group -lm -Wl,--end-group -mthumb -Wl,--print-memory-usage -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m0plus)
add_link_options(-T ${LINKER_SCRIPT})


add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
target_link_libraries(${PROJECT_NAME}.elf PUBLIC Universal_hall)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC FreeRTOS)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC fram_driver)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC sensor_compression)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC sensor_fingerposition)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC sensor_ventilation)


set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

add_subdirectory(board_sdk)
add_subdirectory(lib)