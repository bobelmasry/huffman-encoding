#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include "../huffmantree.h"
using namespace std;
bool areFilesIdentical(const string& filePath1, const string& filePath2) {
    ifstream file2(filePath2, ios::binary);
    ifstream file1(filePath1,ios::binary);
    if (!file1.is_open() || !file2.is_open()) {
        cout << "Error: Could not open one or both files." << endl;
        return false; // Return false if either file cannot be opened
    }
    
    // Compare file sizes first (optional, but more efficient)
    file1.seekg(0, ios::end);
    file2.seekg(0, ios::end);
    streampos size1 = file1.tellg();
    streampos size2 = file2.tellg();
    if (size1 != size2) {
        cout << "Files are different sizes. Size1: " << size1 << ", Size2: " << size2 << endl;
        return false; 
    }

    // Return to beginning of files
    file1.seekg(0, ios::beg);
    file2.seekg(0, ios::beg);

    // Compare files character by character
    char char1, char2;
    while (file1.get(char1) && file2.get(char2)) {
        if (char1 != char2) {
            return false; 
        }
    }

    // Make sure both files have reached the end
    if (file1.get(char1) || file2.get(char2)) {
        cout << "Files have different lengths after reaching EOF. One file still has more data." << endl;
        return false;
    }

    cout << "Files are identical." << endl;
    return true;
}

TEST(FileComparatorTest, FilesAreIdentical) {

    string filePath1 = "inputText.txt";
    string filePath2 = "Decompressed.txt";
    ofstream outfile(filePath1);

 outfile << "Lorem Ipsum is simply dummy text of the printing and typesetting industry.\n Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the\n release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum." << std::endl;
    outfile.close();
      HuffmanTree huffmanTree;
    huffmanTree.Zip("inputText");
    huffmanTree.UnZip("inputText.huff");
    ASSERT_TRUE(areFilesIdentical(filePath1, filePath2)) << "Files are not identical!";
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
