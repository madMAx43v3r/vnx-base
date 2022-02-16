#!/bin/bash

MAKE_NCPU=${MAKE_NCPU:-"8"}

mkdir -p build

cd build

cmake -D VNX_BUILD_TESTS=ON -D CMAKE_CXX_FLAGS="-g -fmax-errors=1 -Wsuggest-override" ..

make -j$MAKE_NCPU $@

