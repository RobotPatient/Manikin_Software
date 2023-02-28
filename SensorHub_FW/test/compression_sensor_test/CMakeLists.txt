set(This compression_sensor_mock_test)

set(Sources
        i2c_helper.hpp
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/i2c/testing/i2c_testclass.hpp
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/hal_sensors/sensor_compression/drivers/VL6180x.cpp
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/hal_sensors/sensor_compression/drivers/VL6180x.hpp
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/hal_sensors/sensor_compression/sensor_compression.hpp
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/hal_sensors/sensor_compression/sensor_compression.cpp
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/hal_sensors/sensor_base/sensor_base.hpp
        compression_sensor_mock_test.cc
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)

target_include_directories(${This} PUBLIC ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/i2c/testing/
                                          ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/hal_sensors/sensor_compression
                                          ${PROJECT_SOURCE_DIR}/.pio/libdeps/samd21/Sensor/modules/hal_sensors/sensor_base
                                          .)
add_test(
        NAME ${This}
        COMMAND ${This}
)