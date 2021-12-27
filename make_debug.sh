#!/bin/bash

MAKE_NCPU=${MAKE_NCPU:-"8"}

mkdir -p build

cd build

cmake -D CMAKE_CXX_FLAGS="-g -fmax-errors=1" ..

make -j$MAKE_NCPU $@

