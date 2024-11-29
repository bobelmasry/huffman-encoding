#include <fstream>
#include "huffmantree.h"

using namespace std;
int main()
{
    HuffmanTree huffmanTree;

    // testing the tree
    huffmanTree.getFrequency();
    huffmanTree.buildHuffmanTree();
    huffmanTree.PrintCodes();
    huffmanTree.PrintFrequency();
    huffmanTree.CompressFileData();

    huffmanTree.SaveCompressedFile("compressed.bin");
    return 0;
}
