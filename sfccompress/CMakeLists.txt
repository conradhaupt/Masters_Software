cmake_minimum_required(VERSION 3.10)

project(
  sfccompress
  VERSION 1.0
  DESCRIPTION "Program that manages sfc compress files"
  LANGUAGES C CXX)


# Option to link to stdc++fs. This is required if GCC8 is being used.
set(LINK_STD_CPP_FS ON CACHE BOOL "Whether to link to stdc++fs to enable std::filesystem usage")

set(LIBSFCCOMPRESS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBSFCCOMPRESS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(
  libsfccompress
  # Source files
  ${LIBSFCCOMPRESS_SOURCE_DIR}/sfcc.cpp
  ${LIBSFCCOMPRESS_SOURCE_DIR}/compressor.cpp
  ${LIBSFCCOMPRESS_SOURCE_DIR}/huffman.cpp
  ${LIBSFCCOMPRESS_SOURCE_DIR}/lzw.cpp
  ${LIBSFCCOMPRESS_SOURCE_DIR}/bitstream.cpp
  # ${LIBSFCCOMPRESS_SOURCE_DIR}/rle.cpp
  # Header files
  ${LIBSFCCOMPRESS_INCLUDE_DIR}/bwt.h
  ${LIBSFCCOMPRESS_INCLUDE_DIR}/compress_handler.h
  ${LIBSFCCOMPRESS_INCLUDE_DIR}/compressor.h
  ${LIBSFCCOMPRESS_INCLUDE_DIR}/huffman.h
  ${LIBSFCCOMPRESS_INCLUDE_DIR}/mtf.h
  ${LIBSFCCOMPRESS_INCLUDE_DIR}/reorder.h
  ${LIBSFCCOMPRESS_INCLUDE_DIR}/sfcc.h
  ${LIBSFCCOMPRESS_INCLUDE_DIR}/sfccexcept.h)
set_target_properties(libsfccompress PROPERTIES PREFIX "")
add_executable(
  sfccompress
  # Source files
  ${LIBSFCCOMPRESS_SOURCE_DIR}/main.cpp)

add_library(sfc::libsfccompress ALIAS libsfccompress)
add_executable(sfc::sfccompress ALIAS sfccompress)

# Handle include paths
target_include_directories(
  libsfccompress
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(
  sfccompress
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(
  libsfccompress PUBLIC cxx_std_17 # Require C++17 for some features
)

target_compile_features(
  sfccompress PUBLIC cxx_std_17 # Require C++17 for some features
)

# Optimise to level 3 if using GNU compilers
target_compile_options(libsfccompress PRIVATE $<$<CXX_COMPILER_ID:GNU>:-O3>)
target_compile_options(sfccompress PRIVATE $<$<CXX_COMPILER_ID:GNU>:-O3>)

# Save the assembly output for debugging purposes Note: Does not modify the
# resulting output library
if(SFC_GENERATE_ASM)
  target_compile_options(libsfccompress PRIVATE -fverbose-asm -save-temps)
  target_compile_options(sfccompress PRIVATE -fverbose-asm -save-temps)
endif()

# Link to libsfc library, throw error if not found
if(NOT TARGET libsfc::libsfc) # Check if the libsfc was found
  message(FATAL_ERROR "sfccompress could not find libsfc")
else()
  message(STATUS "sfccompress found libsfc")
endif()

if(NOT TARGET args)
  message(FATAL_ERROR "sfccompress could not find args")
else()
  message(STATUS "sfccompress found args")
endif()

target_link_libraries(libsfccompress PUBLIC libsfc)
target_link_libraries(sfccompress PUBLIC libsfccompress)
target_link_libraries(libsfccompress PUBLIC args)

if(LINK_STD_CPP_FS)
  message(STATUS "Linking to stdc++fs")
  target_link_libraries(libsfccompress PRIVATE "stdc++fs")
else()
  message(STATUS "Not linking to stdc++fs. Enable LINK_STD_CPP_FS if a compilation error occurs relating to std::filesystem.")
endif()

# Add tests (unit)
add_subdirectory(tests)
