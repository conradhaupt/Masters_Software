cmake_minimum_required(VERSION 3.10)

project(FOLD_EXPRESSION_SUPPORT CXX)

add_executable(FOLD_EXPRESSION_SUPPORT fold_expressions.cpp)

target_compile_features(FOLD_EXPRESSION_SUPPORT PRIVATE cxx_std_17)
