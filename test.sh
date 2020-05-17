#!/bin/bash

cd build
make -j `nproc`
cd ..

while [ true ];
do
    ./build/Debug/bin/render-objects ./projects/3d-tic-tac-toe/output
done