#!/bin/bash

MAKE_NCPU=${MAKE_NCPU:-"8"}

mkdir -p build

cd build

cmake -D VNX_BUILD_TESTS=ON -D CMAKE_CXX_FLAGS="-g -O3 -fmax-errors=1" ..

make -j$MAKE_NCPU $@

