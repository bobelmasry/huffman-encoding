

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

void HuffmanTree::fillPQ()
{
    for (const auto &pair : letterFrequency) {
        cout << pair.first << ": " << pair.second << endl;
        pq.push({pair.first, pair.second});}
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



