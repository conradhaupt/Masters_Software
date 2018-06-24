cmake_minimum_required(VERSION 3.10)

project(libsfc
  VERSION
    0.1
  DESCRIPTION
    "Library that impliments numerous space-filling curves"
  LANGUAGES
    C CXX
)

set(LIBSFC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/libsfc)
set(LIBSFC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(libsfc
  ## Headers
  ${LIBSFC_INCLUDE_DIR}/base2.h
  ${LIBSFC_INCLUDE_DIR}/folds.h
  ${LIBSFC_INCLUDE_DIR}/hilbert.h
  ${LIBSFC_INCLUDE_DIR}/hypercube.h
  ${LIBSFC_INCLUDE_DIR}/interleave.h
  ${LIBSFC_INCLUDE_DIR}/multidim.h
  ${LIBSFC_INCLUDE_DIR}/pow_meta.h
  ${LIBSFC_INCLUDE_DIR}/range.h
  ${LIBSFC_INCLUDE_DIR}/recsfcurve.h
  ${LIBSFC_INCLUDE_DIR}/sfcdef.h
  ${LIBSFC_INCLUDE_DIR}/sfcurve.h
  ${LIBSFC_INCLUDE_DIR}/type_traits.h
  ${LIBSFC_INCLUDE_DIR}/zorder.h

  # Cpp Code
  ${LIBSFC_SOURCE_DIR}/main.cpp
)

add_library(libsfc::libsfc ALIAS libsfc)    # Library alias

# Handle include paths
target_include_directories(libsfc
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Modify target compiler properties
target_compile_features(libsfc
  PUBLIC
    cxx_std_17                              # Require C++17 for some features
)

target_compile_options(libsfc
  PRIVATE $<$<CXX_COMPILER_ID:GNU>:-O2>     # Enable optimisation flag for GNU compilers
)

set_target_properties(libsfc
  PROPERTIES
    CMAKE_CXX_STANDARD_REQUIRED ON          # Use std-c++ instead of gnu++
    CMAKE_CXX_EXTENSIONS OFF
)
