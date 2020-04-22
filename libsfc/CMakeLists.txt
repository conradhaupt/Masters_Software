cmake_minimum_required(VERSION 3.10)

project(
  libsfc
  VERSION 1.0
  DESCRIPTION "Library that impliments numerous space-filling curves"
  LANGUAGES C CXX)

set(LIBSFC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/libsfc)

add_library(libsfc INTERFACE)

add_library(libsfc::libsfc ALIAS libsfc) # Library alias

# Link to OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(libsfc INTERFACE OpenMP::OpenMP_CXX)
endif()

# Handle include paths
target_include_directories(
  libsfc INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                   $<INSTALL_INTERFACE:include>)

# Modify target compiler properties
target_compile_features(
  libsfc INTERFACE cxx_std_17 # Require C++17 for some features
)

# Link to libhilbert for Hilbert mapping
target_link_libraries(libsfc INTERFACE libhilbert)

# Add tests (compiler, unit)
add_subdirectory(tests)

# Handle BMI2 support
if(BMI2_SUPPORTED)
  target_compile_options(libsfc INTERFACE -mbmi2)
  target_compile_definitions(libsfc INTERFACE __LIBSFC_BMI2__)
  message(STATUS "BMI2 supported, enabling")
else()
  message(STATUS "BMI2 not supported, linking to libmorton")
  target_link_libraries(libsfc INTERFACE libmorton)
endif()

# Handle fold-expression support
target_compile_definitions(libsfc INTERFACE __LIBSFC_FOLD__)
message(STATUS "Fold-Expressions supported, enabling")
