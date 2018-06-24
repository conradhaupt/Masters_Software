cmake_minimum_required(VERSION 3.10)

project(BMI2_SUPPORT CXX)

add_executable(BMI2_SUPPORT bmi2.cpp)

target_compile_options(BMI2_SUPPORT PRIVATE -mbmi2)
