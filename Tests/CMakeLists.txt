# Include GoogleTest
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing (this is necessary to enable CTest)
enable_testing()

# Create test executable
add_executable(
    file_comparator_test
    file_comparator_test.cpp
    ../huffmantree.cpp  # Add the Huffman tree source file
    ../lib/priorityqueue.cpp  # Include the priority queue source
)

# Link Google Test
target_link_libraries(
    file_comparator_test
    gtest_main
)

# Register the test with CTest
add_test(NAME FileComparatorTest COMMAND file_comparator_test)
