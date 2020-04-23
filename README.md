# Multi-Dimensional Digital Compression Methods Based On Space-Filling Curves

[![Build Status](https://travis-ci.com/conradhaupt/Masters_Software.svg?token=DCNTppJFLpca6782k9kK&branch=master)](https://travis-ci.com/conradhaupt/Masters_Software)

This program compares numerous space-filling curves using the following metrics: number of discontinuities,
localisation, and continuous lengths in a straight line.

## Project Directory Structure

The project is organised into sub-projects. libsfc is the primary space-filling curve library which contains code defining SFCs, n-dimensional coordinates etc., and locality metrics. The sfccompare project manages processing the locality metrics for given SFCs. The most important executable is sfccompress which facilitates the compression of sfcc files. The folder structure is given below.

```bash
.
├── cmake                         #  CMake config dependencies
├── examples                      #  Examples describing usage of executables
│   └── files                     #  Sample sfcc files from HDF5 and SRTM30 datasets
├── external                      #  External dependencies' CMake configs
│   └── cmake
├── libsfc                        #  Main SFC library
│   ├── include
│   ├── src
│   └── tests                     #  GTests for main SFC library
├── sfccompare                    #  Executable to run some locality metrics
│   ├── src
│   └── tools                     #  Tools for comparing locality metrics
├── sfccompress                   #  Executable to compress sfcc files
│   ├── include
│   ├── src
│   └── tests                     #  GTests for compression executable
└── tools                         #  Tools to manage sfcc files
```

**Note: `sfccompare` is not fully completed and some metrics may not be fully functional.**

## Requirements

- CMake 3.10+
- C/C++ compiler, such as GCC Version 9.x, with compatibility for:
  - BMI2 (Intel 2013 chips and newer) [Optional]
  - Some C++17 features.
  - **NOTE:** Current CMakeLists.txt may not support non-GNU compiler arguments for the required features.
- Git to download dependencies.

## Building

### Build System

- SFCCompare uses CMake to manage dependencies etc.
- Some dependencies are sourced using CMake and [DownloadProject](https://github.com/Crascit/DownloadProject).

### Compiling

Run the following in a standard Linux terminal:

```bash
mkdir build
cd build
cmake ..
make -j
```

### Compiled Output

There are five binary output files from this repo. Four are executables and one is a static library.
There are two unit-testing executables, sfc_test and sfccompress_test, which are
located in `%BUILD_DIR/{libsfc,sfccompress}/tests/unit_tests/`. The main programs
are present in `$BUILD_DIR/sfccompare` and `%BUILD_DIR/sfccompress`. The static
library contains the main implementation for sfccompress in
`%BUILD_DIR/sfccompress/libsfccompress.a`.

**There is no `install` target, requiring that the binaries are manually installed.**

## Dependencies

The following dependencies are downloaded during the configure stage of the build process. See `./external/CMakeLists.txt` for more details.

1. Taywee/args: [https://github.com/Taywee/args](https://github.com/Taywee/args)
2. Google Test (Gtest): [https://github.com/google/googletest](https://github.com/google/googletest)
3. libhilbert: [https://github.com/conradhaupt/libhilbert](https://github.com/conradhaupt/libhilbert)
   - Original: [https://github.com/aipescience/libhilbert](https://github.com/aipescience/libhilbert)
4. libmorton: [https://github.com/conradhaupt/libmorton](https://github.com/conradhaupt/libmorton)
   - Original: [https://github.com/Forceflow/libmorton](https://github.com/Forceflow/libmorton)
5. DownloadProject: [https://github.com/Crascit/DownloadProject](https://github.com/Crascit/DownloadProject)

## License

See the LICENSE file for the program's licensing. Dependencies are under their own licenses, listed in LICENSE. Some source-files have their own license listed in the header. Source-files under their own license are listed in `LICENSE`.
