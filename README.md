# Cipher_crack

[![Build Status](https://travis-ci.org/CppHackers/Cipher_crack.svg?branch=master)](https://travis-ci.org/CppHackers/Cipher_crack)

Программа, позволяющая зашифровывать, расшифровывать и взламывать тексты методами криптоанализа.

## Шифры, с которыми может работать программа:

1) Шифр Цезаря

2) Афинный шифр

3) Шифр простой замены

4) Шифр Полибия

5) Шифр Виженера

## Методы криптоанализа, использующиеся в программе:

1) Брутфорс

2) Частотный анализ букв

3) Частотный анализ n-грамм

4) Поиск восхождением к вершине

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

## Usage

```ShellSession
$ ./WordsLadder [general options] [cipher options] [encrypt/decrypt/crack options]
```

To see more detailed information, run:

```ShellSession
$ ./WordsLadder -h
```
