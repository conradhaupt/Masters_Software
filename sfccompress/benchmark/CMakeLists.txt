# Unit tests
add_executable(sfccompress_benchmark src/bittranspose.cpp src/main.cpp)

if(TARGET libsfccompress)
  message(STATUS "libsfccompress found")
else()
  message(FATAL_ERROR "libsfccompress NOT found")
endif()

target_include_directories(sfccompress_benchmark PRIVATE include)

target_link_libraries(sfccompress_benchmark PUBLIC libsfccompress)

# Use google benchmark
target_link_libraries(sfccompress_benchmark PUBLIC benchmark::benchmark)

# Set optimization level
target_compile_options(sfccompress_benchmark
                       PRIVATE $<$<CXX_COMPILER_ID:GNU>:-O3>)
