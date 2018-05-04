#!/bin/sh

cd $(dirname $0)

if ! command -v cmake > /dev/null 2>&1; then
    echo "Please install cmake"
    exit 1
fi

if [ ! -d build ]; then
    mkdir build
fi

cd build
cmake ../src
