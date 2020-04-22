cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# include(ExternalProject) # Use ExternalProject module for git modules
# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
  message(
    FATAL_ERROR
      "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles."
  )
endif()

# Add ./cmake to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Git submodules
find_package(Git REQUIRED)

project(
  SFCMasters
  VERSION 1.0
  DESCRIPTION
    "A program that compares different properties of various space-filling curves"
  LANGUAGES C CXX)

# Options
option(SFC_GENERATE_ASM "generate assembly code to debug optimisations" OFF)
option(SFC_COMPILE_GTEST_IF_NOT_FOUND
       "compile googletest if not found using FindGTest" ON)
option(BUILD_TESTS # It is the responsability of testing modules to check this
       "compile unit tests" ON)

if(BUILD_TESTS)
  enable_testing()
endif()

# CMake sub-projects
add_subdirectory(external) # Stores external dependencies
add_subdirectory(libsfc) # Main implementation of SFCs
add_subdirectory(sfccompare) # Wrapper program that compares SFCs from libsfc
add_subdirectory(sfccompress) # Program that manages SFC Compressed files
