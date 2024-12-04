cmake_minimum_required(VERSION 3.28)
project(huffman_encoding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add priorityqueue as a library
add_library(priorityqueue STATIC
    priorityqueue.cpp  # The source file for the priority queue
)

# Add the executable for the main program
add_executable(
    huffman_encoding
    main.cpp
    huffmantree.h
    huffmantree.cpp
    priorityqueue.h 
    priorityqueue.cpp

)

# Link the priorityqueue library to the executable
target_link_libraries(huffman_encoding PRIVATE priorityqueue)

# Include the header files for the target (this ensures it can find priorityqueue.h)


# Enable testing (if you have tests)
enable_testing()
add_subdirectory(Tests)
