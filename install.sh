#!/bin/bash
mkdir build 
cd build
cmake .. 
make
./Tests
ln ./Cipher_crack /usr/local/bin/crack
cd ..
cp crack.1.gz /usr/share/man/man1/crack.1.gz
