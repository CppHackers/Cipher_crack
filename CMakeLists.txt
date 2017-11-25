cmake_minimum_required(VERSION 3.8)
project(Cipher_crack)

set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

set(SOURCE_FILES include/Cipher.hpp include/data_alphabet.hpp include/Caesar.hpp include/Affine.hpp include/SimpleSubstitution.hpp
        sources/Cipher.cpp sources/main.cpp sources/Caesar.cpp sources/Affine.cpp sources/SimpleSubstitution.cpp)
add_executable(Cipher_crack ${SOURCE_FILES})