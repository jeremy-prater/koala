#!/bin/bash

DEPS=( "corrade" "magnum" )

for DEP in "${DEPS[@]}"
do
    echo "=========> Build : Erasing ${DEP}"
    rm -rf ${DEP}
    echo "=========> Build : Linking ${DEP}"
    cp -r ../${DEP} .
done

echo "=========> Build : Destroying build"
rm -rf build
mkdir build

echo "=========> Build : Generating new build"
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .