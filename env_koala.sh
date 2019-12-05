#!/bin/bash

git submodule init
git submodule update
git submodule sync

echo "=========> Build : Destroying build"
rm -rf build
mkdir build

echo "=========> Build : Generating new build"
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j 24
