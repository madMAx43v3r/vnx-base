#!/bin/bash

MAKE_NCPU=${MAKE_NCPU:-"8"}

mkdir -p build

cd build

if [ `uname -o` == "Msys" ]; then
	cmake -G "MSYS Makefiles" -D VNX_BUILD_TOOLS=ON -D CMAKE_BUILD_TYPE=Release ..
else
	cmake -D VNX_BUILD_TOOLS=ON -D CMAKE_BUILD_TYPE=Release ..
fi

make -j$MAKE_NCPU $@

