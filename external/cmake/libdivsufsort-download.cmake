cmake_minimum_required(VERSION 2.8.2)

project(libdivsufsort-downloadproj NONE)

download_project(PROJ libdivsufsort
                 GIT_REPOSITORY https://github.com/y-256/libdivsufsort.git
                 GIT_TAG master
                 UPDATE_DISCONNECTED ON
                 CMAKE_ARGS "-DBUILD_SHARED_LIBS=OFF"
                 )
