#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include "huffmantree.h"

using namespace std;

HuffmanTree::HuffmanTree(){};
HuffmanTree::~HuffmanTree(){};
HuffmanTree::HuffmanTree(const HuffmanTree& other) {
    letterFrequency = other.letterFrequency;
    line = other.line;
    pq = other.pq;
};
// get letter frequencies
void HuffmanTree::getFrequency(){
    ifstream inputFile("inputText.txt");
    if (!inputFile) {
        cout << "Error opening Input file" << endl;
        return;
    }
    while (getline(inputFile, line)) {
        for (char ch : line) {
            if (isalpha(ch)) {
                ch = tolower(ch);
                letterFrequency[ch]++;
            }
        }
    }
    inputFile.close();
}
// fill the priority queue
void HuffmanTree::fillPQ()
{
    for (const auto &pair : letterFrequency) {
        cout << pair.first << ": " << pair.second << endl;
        pq.push({pair.first, pair.second});}
}

void HuffmanTree::buildHuffmanTree() {
    while (pq.size() > 1) {
        letter* left = new letter(pq.top()); pq.pop();
        letter* right = new letter(pq.top()); pq.pop();

        // create a combined node
        letter* combined = new letter('\0', left->frequency + right->frequency); //'\0' here is a null character
        combined->left = left;
        combined->right = right;

        pq.push(*combined);
    }
    letter* root = new letter(pq.top()); pq.pop();

    this->root = root;
}

void HuffmanTree::output()
{
    ofstream outputFile("outputFrequency.txt");
    if (outputFile) {
        for (const auto &pair : letterFrequency) {
            outputFile << pair.first << ": " << pair.second << endl;
        }
        outputFile.close();
    } else {
        cout << "Error opening output file" << endl;
    }
}
