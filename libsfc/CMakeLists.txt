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
  ${LIBSFC_INCLUDE_DIR}/interleave.h
  ${LIBSFC_INCLUDE_DIR}/morton.h
  ${LIBSFC_INCLUDE_DIR}/pow.h
  ${LIBSFC_INCLUDE_DIR}/range.h
  ${LIBSFC_INCLUDE_DIR}/sfcdef.h
  ${LIBSFC_INCLUDE_DIR}/sfcurve.h
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

# Save the assembly output for debugging purposes
# Note: Does not modify the resulting output library
if(SFC_GENERATE_ASM)
  target_compile_options(libsfc
    PRIVATE
    -fverbose-asm
    -save-temps
  )
endif()

## Add tests (compiler, unit, benchmark)
add_subdirectory(tests)

# Handle BMI2 support
if(BMI2_SUPPORTED)
  target_compile_options(libsfc PUBLIC -mbmi2)
  target_compile_definitions(libsfc PUBLIC __LIBSFC_BMI2__)
  message(STATUS "BMI2 supported, enabling")
else()
  # Throw error instead of warning until a work-around is implemented
  # message(WARNING "BMI2 not supported, using alternative functions")
  message(FATAL_ERROR "BMI2 not supported")
endif()

# Handle fold-expression support
if(FOLD_EXPRESSION_SUPPORTED)
  target_compile_definitions(libsfc PUBLIC __LIBSFC_FOLD__)
  message(STATUS "Fold-Expressions supported, enabling")
else()
  # No known work-around for fold-expressions, throw an error instead of warning
  # message(WARNING "Fold-Expressions not supported, using alternative functions")
  message(FATAL_ERROR "Fold-Expressions not supported")
endif()
