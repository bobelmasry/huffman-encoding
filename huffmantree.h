#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
using namespace std;

class HuffmanTree {
private:

    struct letter {
        char ch;
        int frequency;
        bool operator<(const letter &other) const;
    };

    map<char, int> letterFrequency;
    string line;
    priority_queue<letter> pq;

    void getFrequency(); //get the frequency of all letters and store them in a map
    void fillPQ(); //fill the priority queue
    void buildHuffmanTree(); //transforms the priority queue into a tree
    void output(); //outputs to text file


public:
    HuffmanTree();
    ~HuffmanTree();
    HuffmanTree(const HuffmanTree& other);
    // HuffmanTree& operator=(const HuffmanTree& other) {
    //     return *this;
    // };
};

#endif //HUFFMANTREE_H

