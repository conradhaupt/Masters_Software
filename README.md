# SFCCompare

This program compares numerous space-filling curves using the following metrics: number of discontinuities,
localisation, and continuous lengths in a straight line.

## Project Directory Structure

The project is organised into sub-projects: the most important being libsfc. libsfc holds the space-filling curve implementations. The second sub-project is sfrccompare which implements the metric comparison using libsfc.

```bash
.
├── CMakeLists.txt      #  project build file
├── libsfc              #  main SFC library
│   ├── CMakeLists.txt  #   > library build file
│   ├── include         #   > public headers
│   │   └── libsfc      #     - sub-dir for library
│   └── src             #   > private code and headers
└── sfccompare          #  main SFCCompare program
    ├── CMakeLists.txt  #   > program build file
    ├── include         #   > public headers
    └── src             #   > private code and headers
```

## Build System

SFCCompare uses CMake to manage dependencies etc.

## Requirements

- CMake 3.10+
- C/C++ compiler with compatibility for:
  - BMI2 (Intel 2013 chips and newer)
  - Some C++17 features
  - **NOTE:** Current CMakeLists.txt may not support non-GNU compiler arguments for the required features.

## Dependencies

The following dependencies are downloaded during the configure stage of the build process. See ```./external/CMakeLists.txt``` for more details.

- Google Test
  - [https://github.com/google/googletest](https://github.com/google/googletest)
- Google Benchmark Framework
  - [https://github.com/google/benchmark](https://github.com/google/benchmark)
- libhilbert
  - [https://github.com/conradhaupt/libhilbert](https://github.com/conradhaupt/libhilbert)

## Building

Run the following:

```bash
mkdir build
cd build
cmake ..
make
./sfccompare/sfccompare
```

## License

See the LICENSE file for the program's licensing.
