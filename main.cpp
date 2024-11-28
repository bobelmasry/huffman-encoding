#include <fstream>
#include "huffmantree.h"

using namespace std;
int main() {
    HuffmanTree huffmanTree;

    // testing the tree
    huffmanTree.getFrequency();

    huffmanTree.fillPQ();

    huffmanTree.buildHuffmanTree();
    huffmanTree.outputfreq();
huffmanTree.EncodeAndSave();
huffmanTree.LoadAndDecode();

    return 0;
}



