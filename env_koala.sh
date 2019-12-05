#!/bin/bash

echo "=========> Build : Destroying build"
rm -rf build
mkdir build

echo "=========> Build : Generating new build"
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j 24