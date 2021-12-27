#!/bin/bash

VERSION=$(cat VERSION)
ARCH=$(g++ -dumpmachine | cut -d- -f1)
DIST="windows"
DST=vnx-base-$VERSION-$ARCH-$DIST

./codegen.sh || exit 1

rm -rf $DST
mkdir -m 755 -p $DST/usr
mkdir -p $ARCH

./install.sh -DCMAKE_INSTALL_PREFIX:PATH=$PWD/$DST || exit 1

7z a ${DST}.7z ${DST} || exit 1
mv ${DST}.7z $ARCH/
rm -rf $DST

