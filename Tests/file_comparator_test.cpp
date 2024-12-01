#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <iostream>

bool areFilesIdentical(const std::string& filePath1, const std::string& filePath2) {
    std::ifstream file1(filePath1, std::ios::binary);
    std::ifstream file2(filePath2, std::ios::binary);

    if (!file1.is_open() || !file2.is_open()) {
        std::cout << "Error: Could not open one or both files." << std::endl;
        return false; // Return false if either file cannot be opened
    }

    // Compare file sizes first (optional, but more efficient)
    file1.seekg(0, std::ios::end);
    file2.seekg(0, std::ios::end);
    std::streampos size1 = file1.tellg();
    std::streampos size2 = file2.tellg();
    if (size1 != size2) {
        std::cout << "Files are different sizes. Size1: " << size1 << ", Size2: " << size2 << std::endl;
        return false; // Files are different sizes
    }

    // Return to beginning of files
    file1.seekg(0, std::ios::beg);
    file2.seekg(0, std::ios::beg);

    // Compare files character by character
    char char1, char2;
    int position = 0; // To track character position in the file
    while (file1.get(char1) && file2.get(char2)) {
        if (char1 != char2) {
            std::cout << "Files differ at position " << position << ". Char1: '" << char1 << "', Char2: '" << char2 << "'" << std::endl;
            return false; // Return false if any character is different
        }
        position++;
    }

    // Make sure both files have reached the end
    if (file1.get(char1) || file2.get(char2)) {
        std::cout << "Files have different lengths after reaching EOF. One file still has more data." << std::endl;
        return false; // If one file has more data left
    }

    std::cout << "Files are identical." << std::endl;
    return true;
}

TEST(FileComparatorTest, FilesAreIdentical) {
    std::string filePath1 = "inputText.txt";
    std::string filePath2 = "Decompressed.txt";

    ASSERT_TRUE(areFilesIdentical(filePath1, filePath2)) << "Files are not identical!";
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
