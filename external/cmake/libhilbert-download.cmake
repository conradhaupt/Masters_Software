cmake_minimum_required(VERSION 2.8.2)

project(libhilbert-downloadproj NONE)

download_project(PROJ
                 libhilbert
                 GIT_REPOSITORY
                 https://github.com/conradhaupt/libhilbert
                 GIT_TAG
                 master
                 UPDATE_DISCONNECTED
                 ON)
