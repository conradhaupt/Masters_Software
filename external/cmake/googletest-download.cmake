cmake_minimum_required(VERSION 2.8.2)

project(googletest-downloadproj NONE)

download_project(PROJ                 googletest
                 GIT_REPOSITORY       https://github.com/google/googletest.git
                 GIT_TAG              master
                 UPDATE_DISCONNECTED  ON
                 CMAKE_ARGS           "-Dgtest_force_shared_crt=ON"

)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
