cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

include(ExternalProject) # Use ExternalProject module for git modules
include(GNUInstallDirs) # Use GNUInstallDirs to handle directory processing

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

# This is commented out as this project is under a larger git project (Masters
# work)

# if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
if(GIT_FOUND)
  # Update submodules as needed
  option(GIT_SUBMODULE "Check submodules during build" ON)
  if(GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
                            --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(
        SEND_ERROR
          "git submodule update --init failed with '${GIT_SUBMOD_RESULT}', please checkout submodules"
        )
    endif()
  endif()
endif()

project(
  SFCCompare
  VERSION
    0.1 DESCRIPTION
    "A program that compares different properties of various space-filling curves"
  LANGUAGES C CXX)

# Options
option(SFC_GENERATE_ASM "generate assembly code to debug optimisations" ON)
option(SFC_COMPILE_GTEST_IF_NOT_FOUND
       "compile googletest if not found using FindGTest" ON)
option(SFC_COMPILE_BENCHMARK_IF_NOT_FOUND # Note: Depends on GTest
       "compile google benchmark if not found using FindBenchmark"
       ON)
option(BUILD_TESTS # It is the responsability of testing modules to check this
                   # option
       "compile unit tests"
       ON)
option(BUILD_BENCHMARKS # It is the responsability of benchmarking modules to
                        # check this option
       "compile benchmarks"
       OFF)

if(BUILD_TESTS)
  include(CTest)
  enable_testing()
endif()

# CMake sub-projects
add_subdirectory(external) # Stores external dependencies
add_subdirectory(libsfc) # Main implementation of SFCs
add_subdirectory(sfccompare) # Wrapper program that compares SFCs from libsfc
