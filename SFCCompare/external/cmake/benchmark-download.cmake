cmake_minimum_required(VERSION 2.8.2)

project(benchmark-downloadproj NONE)

option(BENCHMARK_ENABLE_TESTING "Enable testing of the benchmark library." OFF)

download_project(PROJ                 benchmark
                 GIT_REPOSITORY       https://github.com/google/benchmark.git
                 GIT_TAG              master
                 UPDATE_DISCONNECTED  ON
                 CMAKE_ARGS           "-DCMAKE_CROSSCOMPILING=ON -DBENCHMARK_ENABLE_TESTING=OFF"

)
