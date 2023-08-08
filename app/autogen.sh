#!/bin/sh

autoreconf --install
./configure --enable-gcc-debug  --prefix=$HOME/tmp/split698  
make
make check
