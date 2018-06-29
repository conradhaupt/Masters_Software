# Add *-download.cmake path to CMAKE_MODULE_PATH
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(DownloadProject)

## Add googletest library if BUILD_TESTS is enabled
if(BUILD_TESTS)
# Try find gtest using find_*
set(SFC_GTEST_SEARCH_DIR "" CACHE PATH "dir to search in for googletest")
find_package(GTest HINTS ${SFC_GTEST_SEARCH_DIR})

# If find_* didn't work, try download, build, and integrate with project
if(NOT TARGET gtest_main AND NOT SFC_COMPILE_GTEST_IF_NOT_FOUND)
  message(FATAL_ERROR "Could NOT find GTest. Enable SFC_COMPILE_GTEST_IF_NOT_FOUND to download and install it")
elseif(NOT TARGET gtest_main)
  message(STATUS "GTest not found, using DownloadPackage")
  include(googletest-download)
  # Add googletest subdir
  add_subdirectory(
    ${googletest_SOURCE_DIR}
    ${googletest_BINARY_DIR}
    EXCLUDE_FROM_ALL)

  # The gtest/gtest_main targets carry header search path
  # dependencies automatically when using CMake 2.8.11 or
  # later. Otherwise we have to add them here ourselves.
  if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
  endif()

endif()
if(TARGET gtest_main)
  message(STATUS "Found downloaded GTest")
else()
  message(WARNING "Could NOT find downloaded GTest")
endif() # if target gtest_main found
endif() # if BUILD_TESTS

# Add google benchmark library if BUILD_BENCHMARKS is enabled
if(BUILD_BENCHMARKS)
set(SFC_BENCHMARK_SEARCH_DIR "" CACHE PATH "dir to search in for googletest")
find_package(benchmark HINTS ${SFC_BENCHMARK_SEARCH_DIR})

# If find_* didn't work, try download, build, and integrate with project
if(benchmark_DIR AND NOT TARGET benchmark_main AND NOT SFC_COMPILE_BENCHMARK_IF_NOT_FOUND)
  message(FATAL_ERROR "Could NOT find benchmark. Enable SFC_COMPILE_BENCHMARK_IF_NOT_FOUND to download and install it")
elseif(NOT TARGET benchmark_main)
  message(STATUS "benchmark not found, using ExternalPackage")
  include(benchmark-download)
  # Add benchmark subdir
  add_subdirectory(
    ${benchmark_SOURCE_DIR}
    ${benchmark_BINARY_DIR})
endif()
if(TARGET benchmark_main)
  message(STATUS "Found downloaded benchmark")
else()
  message(FATAL_ERROR "Could NOT find downloaded benchmark")
endif() # if target benchmark_main found
endif() # if BUILD_BENCHMARKS

# # Add CLIPP library headers
# add_library(clipp INTERFACE)
# target_include_directories(clipp INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
