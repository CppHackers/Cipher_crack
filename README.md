# Cipher_crack

[![Build Status](https://travis-ci.org/CppHackers/Cipher_crack.svg?branch=master)](https://travis-ci.org/CppHackers/Cipher_crack)

## What's the Cipher_crack

It's a program that can encrypt, decrypt and crack texts using cryptanalysis.

### Ciphers used:

1) Caesar cipher

2) Affine cipher

3) Simple substitution cipher

4) Polybius cipher

5) Vigenere cipher

### Сryptanalysis methods used:

1) Brute force

2) Frequency analysis of letters

3) Frequency analysis of n-grams

4) 'Hill-climbing' algorithm

5) Accounting for the index of coincidence

## Build

```ShellSession
$ git clone https://github.com/CppHackers/Cipher_crack
$ cd Cipher_crack
$ mkdir build && cd build
$ cmake .. && make
```

## Running tests

```ShellSession
$ ./Tests
```

## How to use it

```ShellSession
$ ./Cipher_crack [general options] [cipher options] [encrypt/decrypt/crack options]
```

## More

```ShellSession
$ ./Cipher_crack -h
```
