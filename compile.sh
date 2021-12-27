#!/bin/bash

git fetch
git checkout $1 || exit

cd codegen
./clean_all.sh
./make_release.sh
cd ..

./clean_all.sh
./codegen.sh
./make_release.sh
./test_all.sh

