# sfz-flat

This is a simple command line utility that takes an input sfz file and outputs its "flat" version, 1 region per line, with every opcode that will apply to the region.
Use it to debug your sfz files and get a chance to see what will be parsed by sfz samplers in the end, since most of them work this way -- flattening and loading regions.

## Installing

Ensure you have a C++14 compiler, and CMake version 3.13 at least.
```
git clone --recursive https://github.com/sfztools/sfz-flat.git
cd sfz-flat
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
make
```

If it complains that it cannot find some CMakeFiles, ensure that you indeed pull the submodules when cloning by running
```
git submodule update --init --recursive
```

## Usage

Just run
```
sfz-flat [sfz file]
```
You can pipe the output from the console to `less` for better readability.