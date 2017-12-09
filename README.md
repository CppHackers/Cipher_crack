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

## Build requirements

Before installing the program, check that `cmake` and `libboost-all-dev` packets are installed.

## Install

```ShellSession
$ git clone https://github.com/CppHackers/Cipher_crack
$ cd Cipher_crack
$ 
$ ./install.sh
```

## How to use it

```ShellSession
$ crack [general options] [cipher options] [encrypt/decrypt/crack options]
```

## More

```ShellSession
$ man crack
```
or
```ShellSession
$ crack --help
```
or
```ShellSession
$ crack -h
```
