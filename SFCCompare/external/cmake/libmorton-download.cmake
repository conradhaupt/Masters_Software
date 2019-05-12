cmake_minimum_required(VERSION 2.8.2)

project(libmorton-downloadproj NONE)

download_project(PROJ
                 libmorton
                 GIT_REPOSITORY
                 https://github.com/conradhaupt/libmorton.git
                 GIT_TAG
                 master
                 UPDATE_DISCONNECTED
                 ON)
