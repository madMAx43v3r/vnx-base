#!/bin/bash

set -e

VERSION=$(cat VERSION)
ARCH=$(g++ -dumpmachine | cut -d- -f1)
DIST=${DIST:-$(cat ~/DIST)}
DST=vnx-base-$VERSION-$ARCH-$DIST

./codegen.sh || exit 1

rm -rf $DST
mkdir -m 755 -p $DST/DEBIAN
mkdir -m 755 -p $DST/usr
mkdir -p $ARCH

echo "Version: $VERSION" >> $DST/DEBIAN/control
cat $ARCH/debian/control >> $DST/DEBIAN/control
cat debian/control >> $DST/DEBIAN/control

./install.sh -DCMAKE_INSTALL_PREFIX:PATH=$PWD/$DST/usr

fakeroot dpkg-deb --build $DST

mv ${DST}.deb $ARCH/

rm -rf $DST

