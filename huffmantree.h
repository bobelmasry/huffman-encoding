#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <map>
#include <string>
using namespace std;
// #include <queue>
#include "lib/priorityqueue.h"

class HuffmanTree
{
private:
    letter *root; // root of the tree
    map<char, int> letterFrequency;
    string line;
    priorityqueue<letter> pq;

public:
    HuffmanTree();
    HuffmanTree(const HuffmanTree &);
    ~HuffmanTree();
    void getFrequency(const string &);                  // get the frequency of all letters and store them in a map
    void buildHuffmanTree();                            // transforms the priority queue into a tree
    void ReBuildHuffmanTree(const map<string, char> &); // transforms the laod file into a tree
    void generateCodeMap(letter *, string, map<char, string> &);
    void Zip(string);
    void EncodeInput(const string &);
    void SaveCompressedFile(const string &, const string &);
    void UnZip(const string &);
    void DecodeCompressedFile(const string &, const string &);
    long long getFileSize(const string &); // added function declaration
    // DEBUGGING FUNCTIONS
    void PrintFrequency();
    void PrintCodes();
};

#endif // HUFFMANTREE_H
