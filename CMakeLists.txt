cmake_minimum_required(VERSION 3.8)
project(Cipher_crack)

set(CMAKE_CXX_STANDARD 11)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES include/Cipher.hpp include/data_alphabet.hpp include/Caesar.hpp include/Affine.hpp include/SimpleSubstitution.hpp include/Polybius.hpp
        sources/Cipher.cpp sources/Caesar.cpp sources/Affine.cpp sources/SimpleSubstitution.cpp sources/Polybius.cpp sources/main.cpp)

add_executable(Cipher_crack ${SOURCE_FILES})
target_link_libraries(Cipher_crack ${Boost_LIBRARIES})