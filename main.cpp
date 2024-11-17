#include <fstream>
#include "huffmantree.h"

using namespace std;
int main() {
    HuffmanTree huffmanTree;

    // testing the tree
    huffmanTree.getFrequency();

    huffmanTree.fillPQ();

    huffmanTree.buildHuffmanTree();

    huffmanTree.printCodes();

    return 0;
}



