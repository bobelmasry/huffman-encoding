cmake_minimum_required(VERSION 3.28)
project(huffman_encoding)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(
    huffman_encoding
    main.cpp
        huffmantree.h
        huffmantree.cpp
        priorityqueue.cpp
        priorityqueue.h)
        enable_testing()
        add_subdirectory(Tests)

