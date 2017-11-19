cmake_minimum_required(VERSION 3.8)
project(Cipher_crack)

set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

set(SOURCE_FILES include/cipher.hpp include/data_alpha.hpp include/Zzezar.hpp
        sources/cipher.cpp sources/main.cpp sources/Zzezar.cpp include/Affine.hpp sources/Affine.cpp)
add_executable(Cipher_crack ${SOURCE_FILES})