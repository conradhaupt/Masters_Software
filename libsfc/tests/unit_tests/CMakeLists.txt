# If BUILD_TESTS is off, do not process the file
if(NOT BUILD_TESTS)
  message(STATUS "BUILD_TESTS disabled: not building libsfc unit tests")
  return()
endif()

include(GoogleTest)

# Unit tests
add_executable(sfc_test
               src/base2.cpp
               src/clusters.cpp
               src/coordinates.cpp
               src/interleave.cpp
               src/iterator.cpp
               src/libhilbert.cpp
               src/main.cpp
               src/pow.cpp
               src/range.cpp
               src/rectangle.cpp
               src/sfcurve.cpp)

# Alias
add_executable(libsfc::sfc_test ALIAS sfc_test)

if(TARGET libsfc)
  message(STATUS "libsfc found")
else()
  message(FATAL_ERROR "libsfc NOT found")
endif()

target_include_directories(sfc_test PRIVATE include)

target_link_libraries(sfc_test libsfc)

# Use googletest for testing
if(TARGET gtest_main)
  target_link_libraries(sfc_test gtest_main)
else()
  message(FATAL_ERROR "googletest not found, required for libsfc testing")
endif()

if(TARGET gmock)
  target_link_libraries(sfc_test gmock)
else()
  message(FATAL_ERROR "googlemock not found, required for libsfc testing")
endif()

gtest_discover_tests(sfc_test)
# add_test(NAME example_test COMMAND sfc_test)
