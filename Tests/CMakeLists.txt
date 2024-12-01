
        include(FetchContent)

        FetchContent_Declare(
          googletest
          URL https://github.com/google/googletest/archive/refs/heads/main.zip
        )
        FetchContent_MakeAvailable(googletest)
        
        enable_testing()
        
        add_executable(
          file_comparator_test
          file_comparator_test.cpp
        )
        
        target_link_libraries(
          file_comparator_test
          gtest_main
        )
        
        add_test(NAME FileComparatorTest COMMAND file_comparator_test)
        