cmake_minimum_required(VERSION 3.28)
project(huffman_encoding)

set(CMAKE_CXX_STANDARD 14)

add_executable(
    huffman_encoding
    main.cpp
        huffmantree.h
        huffmantree.cpp)
        enable_testing()
        add_subdirectory(Tests)