# If BUILD_TESTS is off, do not process the file
if(NOT BUILD_TESTS)
  message(STATUS "BUILD_TESTS disabled: not building libsfc unit tests")
  return()
endif()

include(GoogleTest)

# Unit tests
add_executable(
  sfccompress_test
  src/sfcc.cpp
  src/reorder.cpp
  src/bwt.cpp
  src/lzw.cpp
  src/mtf.cpp
  src/bittranspose.cpp
  src/compressor.cpp
  ../../src/compressor.cpp # not a pretty way of resolving undefined errors for
                           # bzip<> templated types when linking
)

# Alias
add_executable(sfccompress::sfccompress_test ALIAS sfccompress_test)

if(TARGET libsfccompress)
  message(STATUS "libsfccompress found")
else()
  message(FATAL_ERROR "libsfccompress NOT found")
endif()

target_include_directories(sfccompress_test PRIVATE include)

target_link_libraries(sfccompress_test libsfccompress)
target_link_libraries(sfccompress_test divsufsort)
target_compile_definitions(
  sfccompress_test
  PRIVATE
    TEST_COMPRESSOR_DATA_FILENAME="${CMAKE_CURRENT_SOURCE_DIR}/data/test.small.sfcc"
)

# Use googletest for testing
if(TARGET gtest_main)
  target_link_libraries(sfccompress_test gtest_main)
else()
  message(
    FATAL_ERROR "googletest not found, required for libsfccompress testing")
endif()

if(TARGET gmock)
  target_link_libraries(sfccompress_test gmock)
else()
  message(
    FATAL_ERROR "googlemock not found, required for libsfccompress testing")
endif()

gtest_discover_tests(sfccompress_test)
# add_test(NAME example_test COMMAND sfccompress_test)
