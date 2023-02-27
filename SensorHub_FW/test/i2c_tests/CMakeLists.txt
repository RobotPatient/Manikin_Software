set(This i2c_wrapper_mock_test)

set(Sources
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/adafruit_feather_m0_express/I2C_Wrapper/src/i2c_helper.h
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/adafruit_feather_m0_express/I2C_Wrapper/src/i2c_helper.cpp
        ${PROJECT_SOURCE_DIR}/.pio/libdeps/adafruit_feather_m0_express/I2C_Wrapper/src/i2c_testclass.h
        i2c_wrapper_mock_test.cc
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC gtest_main gmock_main)

target_include_directories(${This} PUBLIC ${PROJECT_SOURCE_DIR}/test
                                          ${PROJECT_SOURCE_DIR}/.pio/libdeps/native/I2C_Wrapper/src)
add_test(
        NAME ${This}
        COMMAND ${This}
)