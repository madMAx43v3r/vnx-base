@ECHO OFF

@RD /S /Q build

mkdir build

cmake -B build -G Ninja -D CMAKE_BUILD_TYPE="Release" -D VNX_BUILD_TOOLS=ON -D VNX_BUILD_TESTS=ON
cmake --build build -- -k0
