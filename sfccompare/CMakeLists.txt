cmake_minimum_required(VERSION 3.10)

project(
  sfccompare
  VERSION 1.0
  DESCRIPTION
    "Program that compares multiple metrics of various space-filling curves"
  LANGUAGES C CXX)

set(SFCCOMPARE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SFCCOMPARE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(sfccompare
               # Headers
               # Cpp code
               ${SFCCOMPARE_SOURCE_DIR}/main.cpp)

add_executable(sfc::sfccompare
               ALIAS
               sfccompare # Program alias
               )

# Handle include paths
target_include_directories(
  sfccompare
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
         $<INSTALL_INTERFACE:include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(sfccompare
                        PUBLIC
                        cxx_std_17 # Require C++17 for some features
                        )

# Optimise to level 3 if using GNU compilers
target_compile_options(sfccompare PRIVATE $<$<CXX_COMPILER_ID:GNU>:-O3>)

# Save the assembly output for debugging purposes Note: Does not modify the
# resulting output library
if(SFC_GENERATE_ASM)
  target_compile_options(sfccompare PRIVATE -fverbose-asm -save-temps)
endif()

# Link to libsfc library, throw error if not found
if(NOT TARGET libsfc::libsfc) # Check if the libsfc was found
  message(FATAL_ERROR "sfccompare could not find libsfc")
else()
  message(STATUS "sfccompare found libsfc")
endif()
target_link_libraries(sfccompare PUBLIC libsfc)
