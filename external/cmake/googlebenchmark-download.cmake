cmake_minimum_required(VERSION 2.8.2)

project(googlebenchmark-downloadproj NONE)

download_project(
  PROJ
  googlebenchmark
  GIT_REPOSITORY
  https://github.com/google/benchmark.git
  GIT_TAG
  master
  UPDATE_DISCONNECTED ON
  BENCHMARK_ENABLE_TESTING OFF)

# Prevent GoogleTest from overriding our compiler/linker options when building
# with Visual Studio
# set(gtest_force_shared_crt
#     ON
#     CACHE BOOL "" FORCE)
