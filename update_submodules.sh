#!/bin/bash

git submodule init
git submodule update
git submodule sync

DIRS="corrade magnum magnum-bootstrap magnum-plugins projects external/cppzmq external/libzmq"

for DIR in $DIRS
do
    echo "==> Updating $DIR"
    pushd $DIR
    git reset --hard HEAD
    git checkout master
    git pull
    popd
done