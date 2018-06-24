# SFCCompare

This program compares numerous space-filling curves using the following metrics: number of discontinuities,
localisation, and continuous lengths in a straight line.

## Project Directory Structure

The project is organised into sub-projects: the most important being libsfc. libsfc holds the space-filling curve implementations. The second sub-project is sfrccompare which implements the metric comparison using libsfc.

<pre>
.
├── CMakeLists.txt      # project build file
├── libsfc              # main SFC library
│   ├── CMakeLists.txt    > library build file
│   ├── include           > public headers
│   │   └── libsfc          - sub-dir for library
│   └── src               > private code and headers
└── sfccompare          # main SFCCompare program
    ├── CMakeLists.txt    > program build file
    ├── include           > public headers
    └── src               > private code and headers
</pre>

## Build System

SFCCompare uses CMake to manage dependencies etc.

## Requirements
- Cmake 3.10+
- C/C++ compiler with compatibility for:
  - BMI2 (Intel 2013 chips and newer)
  - Some C++17 features
  - **NOTE:** Current CMakeLists.txt may not support non-GNU compiler arguments for the required features.

## Dependencies
No external dependencies yet. Will add testing frameworks soon.

## Building

Run the following:
```
mkdir build
cd build
cmake ..
make
./sfccompare
```

## License

See the LICENSE file for the program's licensing.
