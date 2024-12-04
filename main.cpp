#include <fstream>
#include "huffmantree.h"
using namespace std;

#include <filesystem>
#include <iostream>

using namespace std;

int main()
{
   HuffmanTree huffmanTree;

    // testing the tree
    huffmanTree.Zip("inputText");
    huffmanTree.UnZip("inputText.huff");
    return 0;
}
