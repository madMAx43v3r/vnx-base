#!/bin/bash

MAKE_NCPU=${MAKE_NCPU:-"8"}

mkdir -p build

cd build

if [ `uname -o` == "Msys" ]; then
	cmake -G "MSYS Makefiles" -D VNX_BUILD_TOOLS=ON -D CMAKE_CXX_FLAGS="-O3" $@ ..
else
	cmake -D VNX_BUILD_TOOLS=ON -D CMAKE_CXX_FLAGS="-O3" $@ ..
fi

make -j$MAKE_NCPU install

