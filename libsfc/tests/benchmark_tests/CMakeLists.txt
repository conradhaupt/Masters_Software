# If BUILD_BENCHMARKS is off, do not process the file
if(NOT BUILD_BENCHMARKS)
  message(STATUS "BUILD_BENCHMARKS disabled: not building libsfc benchmarks")
  return()
endif()

include(GoogleTest)

# Unit tests
add_executable(sfc_bench
  src/main.cpp
)

# Alias
add_executable(libsfc::sfc_bench ALIAS sfc_bench)

# target_link_libraries(sfc_test libsfc::libsfc)

# Use benchmark for benching functionality
if(TARGET benchmark_main)
  target_link_libraries(sfc_bench benchmark_main)
else()
  message(FATAL_ERROR "benchmark not found, required for libsfc benching")
endif()

# add_custom_target(benchmark )
