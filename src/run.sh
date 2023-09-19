#!/bin/bash

mkdir build
cd build

# Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Release
cmake ..
cmake --build.
