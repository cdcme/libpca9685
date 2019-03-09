cmake_minimum_required(VERSION 3.13)

project(libpca9685 VERSION 0.2.1
        DESCRIPTION "Driver for the NXP Semiconductors PCA9685 16-channel 12-bit PWM I2C-bus LED Controller"
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_library(pca9685 STATIC "")
add_library(libpca9685 ALIAS pca9685)
add_subdirectory(include)
add_subdirectory(src)

find_library(MATH m)
target_link_libraries(pca9685 ${MATH})

target_compile_options(pca9685 PUBLIC -W -Wall -Wextra -pedantic)
target_compile_features(pca9685 PUBLIC c_std_11)

if(TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS pca9685 DESTINATION lib)
install(FILES include/pca9685.h DESTINATION include)

