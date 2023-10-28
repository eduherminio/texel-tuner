#!/bin/bash

mkdir build
cd build

# Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
./tuner ../sources.csv

# Release
cmake ..
cmake --build .
./tuner ../sources.csv