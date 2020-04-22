# Add *-download.cmake path to CMAKE_MODULE_PATH
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(DownloadProject)

# Add googletest library if BUILD_TESTS is enabled
if(BUILD_TESTS)
  set(SFC_GTEST_SEARCH_DIR "" CACHE PATH "dir to search in for googletest")

  # If find_* didn't work, try download, build, and integrate with project
  if(NOT TARGET gtest_main AND NOT SFC_COMPILE_GTEST_IF_NOT_FOUND)
    message(
      FATAL_ERROR
        "Could NOT find GTest. Enable SFC_COMPILE_GTEST_IF_NOT_FOUND to download and install it"
      )
  elseif(NOT TARGET gtest_main)
    message(STATUS "GTest not found, using DownloadPackage")
    include(googletest-download)
    # Add googletest subdir
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR}
                     EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path dependencies
    # automatically when using CMake 2.8.11 or later. Otherwise we have to add
    # them here ourselves.
    if(CMAKE_VERSION VERSION_LESS 2.8.11)
      include_directories("${gtest_SOURCE_DIR}/include")
    endif()

  endif()
  if(TARGET gtest_main)
    message(STATUS "Found downloaded GTest")
  else()
    message(WARNING "Could NOT find downloaded GTest")
  endif() # if target gtest_main found
endif() # if BUILD_TESTS

# Add libhilbert library from github.com/conradhaupt/libhilbert
message(STATUS "Invoking libhilbert DownloadPackage")
include(libhilbert-download)
# Add libhilbert subdir
add_subdirectory(${libhilbert_SOURCE_DIR} ${libhilbert_BINARY_DIR}
                 EXCLUDE_FROM_ALL)

include_directories("${libhilbert_SOURCE_DIR}/src")

if(TARGET libhilbert)
  message(STATUS "Found downloaded libhilbert")
else()
  message(FATAL_ERROR "Could NOT find downloaded libhilbert")
endif() # if

# Add libmorton library from github.com/conradhaupt/libmorton
message(STATUS "Invoking libmorton DownloadPackage")
include(libmorton-download)
# Add libmorton subdir
add_subdirectory(${libmorton_SOURCE_DIR} ${libmorton_BINARY_DIR}
                 EXCLUDE_FROM_ALL)

if(TARGET libmorton)
  message(STATUS "Found downloaded libmorton")
else()
  message(FATAL_ERROR "Could NOT find downloaded libmorton")
endif() # if

# Add args library for argument parsing from https://github.com/Taywee/args.git
message(STATUS "Invoking args DownloadPackage")
set(ARGS_BUILD_UNITTESTS OFF CACHE BOOL "Hello")
include(args-download)
# Add args subdir
add_subdirectory(${args_SOURCE_DIR} ${args_BINARY_DIR} EXCLUDE_FROM_ALL)

if(TARGET args)
  message(STATUS "Found downloaded args")
else()
  message(FATAL_ERROR "Could NOT find downloaded args")
endif() # if
