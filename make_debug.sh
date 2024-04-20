#!/bin/bash

MAKE_NCPU=${MAKE_NCPU:-"8"}

mkdir -p build

cd build

cmake -D VNX_BUILD_TESTS=ON -D VNX_BUILD_TOOLS=ON -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_FLAGS="-fmax-errors=1 -Wsuggest-override" ..

make -j$MAKE_NCPU $@

