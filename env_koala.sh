#!/bin/bash

echo "=========> Updating submodules"
python3 ./update_submodules.py

echo "=========> Build : Destroying build"
rm -rf build
mkdir build

HOST_OS=`uname` 
EXTRA_CMAKE=''

if [ $HOST_OS = 'Linux' ]
then
    NUM_CORES=`nproc`
elif [ $HOST_OS = 'Darwin' ]
then
    NUM_CORES=`sysctl -n hw.logicalcpu`
elif [[ $HOST_OS == MINGW* ]]
then
    NUM_CORES=$NUMBER_OF_PROCESSORS
    if [ ! -f "./tools/vcpkg/vcpkg.exe" ]; then
        ./tools/vcpkg/bootstrap-vcpkg.sh
    fi
    ./tools/vcpkg/vcpkg install boost:x64-windows
    ./tools/vcpkg/vcpkg install sdl2:x64-windows
    ./tools/vcpkg/vcpkg install zlib:x64-windows
    ./tools/vcpkg/vcpkg install rapidjson:x64-windows
    EXTRA_CMAKE="-DCMAKE_TOOLCHAIN_FILE=tools/vcpkg/scripts/buildsystems/vcpkg.cmake -DEXTRA_WIN_LIBS=bcrypt"
else
    echo "Unknown OS type! $HOST_OS"
    exit -1
fi

echo "=========> Configuring : Generating new build on $HOST_OS with extra options $EXTRA_CMAKE"
cd build
cmake $EXTRA_CMAKE -DCMAKE_BUILD_TYPE=Debug ..

echo "=========> Build : Generating new build on $HOST_OS using $NUM_CORES"
cmake --build . -j $NUM_CORES
