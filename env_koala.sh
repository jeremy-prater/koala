#!/bin/bash

echo "=========> Updating submodules"
python3 ./update_submodules.py

echo "=========> Build : Destroying build"
rm -rf build
mkdir build

HOST_OS=`uname` 
if [ $HOST_OS = 'Linux' ]
then
    NUM_CORES=`nproc`
elif [ $HOST_OS = 'Darwin' ]
then
    NUM_CORES=`sysctl -n hw.logicalcpu`
else
    echo "Unknown OS type! $HOST_OS"
    exit -1
fi

echo "=========> Build : Generating new build on $HOST_OS using $NUM_CORES cores"
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j $NUM_CORES
