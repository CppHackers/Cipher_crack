#!/bin/bash
mkdir build 
cd build
cmake .. 
make
./Tests
if [ -f "/usr/local/bin/crack" ]; then 
    rm /usr/local/bin/crack
fi
ln ./Cipher_crack /usr/local/bin/crack
cd ..
cp crack.1.gz /usr/share/man/man1/crack.1.gz
