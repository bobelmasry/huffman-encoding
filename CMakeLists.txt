cmake_minimum_required(VERSION 3.10)
project(huffman_encoding)

# Add Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add main executable
add_executable(huffman main.cpp huffmantree.cpp lib/priorityqueue.cpp)

# Add test executable
add_executable(tests Tests/file_comparator_test.cpp huffmantree.cpp lib/priorityqueue.cpp)
target_link_libraries(tests gtest gtest_main)

# Add tests
add_test(NAME tests COMMAND tests)

