cmake_minimum_required(VERSION 2.8.2)

project(args-downloadproj NONE)

download_project(PROJ
                 args
                 GIT_REPOSITORY
                 https://github.com/Taywee/args.git
                 GIT_TAG
                 master
                 UPDATE_DISCONNECTED
                 ON)
