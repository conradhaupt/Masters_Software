# Unit tests
add_executable(sfc_benchmark src/main.cpp )

# Alias
add_executable(libsfc::sfc_benchmark ALIAS sfc_benchmark)

if(TARGET libsfc)
  message(STATUS "libsfc found")
else()
  message(FATAL_ERROR "libsfc NOT found")
endif()

target_include_directories(sfc_benchmark PRIVATE include)

target_link_libraries(sfc_benchmark PUBLIC libsfc)

# Use google benchmark
target_link_libraries(sfc_benchmark PUBLIC benchmark::benchmark)
