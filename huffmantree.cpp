#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include "huffmantree.h"
#include <cstdint>
#include <bitset>
using namespace std;

HuffmanTree::HuffmanTree() {};
HuffmanTree::HuffmanTree(const HuffmanTree &other)
{
    letterFrequency = other.letterFrequency;
    line = other.line;
    pq = other.pq;
};


void HuffmanTree::getFrequency()
{
    ifstream inputFile("inputText.txt");
    if (!inputFile)
    {
        cout << "Error opening Input File" << endl;
        return;
    }
    bool firstline = 1;
    while (getline(inputFile, line))
    {
        for (char ch : line)
        {
            ch = tolower(ch);
            letterFrequency[ch]++;
        }
        if (!firstline)
        {
            letterFrequency['\n']++; // Add newline
        }
        firstline=0; // Add newline
    }
    inputFile.close();
    //Filling the priority queue
    for (const auto &pair : letterFrequency)
    {
        pq.push({pair.first, pair.second});
    }
}

void HuffmanTree::buildHuffmanTree()
{
    while (pq.size() > 1)
    {
        letter *left = new letter(pq.top());
        pq.pop();
        letter *right = new letter(pq.top());
        pq.pop();

        // create a combined node
        letter *combined = new letter('\0', left->frequency + right->frequency); //'\0' here is a null character
        combined->left = left;
        combined->right = right;

        pq.push(*combined);
    }
    letter *root = new letter(pq.top());
    pq.pop();

    this->root = root;
}

void HuffmanTree::generateCodes(letter *node, string code, map<char, string> &codes)
{
    if (!node)
        return;

    // if node is a leaf
    if (node->ch != '\0')
    {
        codes[node->ch] = code;
    }

    // traverse left adding 0 and traversing right adding 1
    generateCodes(node->left, code + "0", codes);
    generateCodes(node->right, code + "1", codes);
}

void HuffmanTree::CompressFileData()
{
    map<char, string> codes;
    generateCodes(this->root, "", codes);

    // Read the input text and encode it
    ifstream inputFile("inputText.txt");
    if (!inputFile)
    {
        cerr << "Error opening inputText.txt for reading" << endl;
        return;
    }

    string compressedData;
    string line;
    bool firstline = 1;

    while (getline(inputFile, line))
    {
        for (char ch : line)
        {
            ch = tolower(ch); // Handle case sensitivity
            compressedData += codes[ch];
            if (ch == ' ') // Add space
            {
                compressedData += codes[' '];
            }
        }
        if (!firstline)
        {
            compressedData += codes['\n']; // Add newline
        }

        firstline = 0;
    }
    cout << "Compressed data: " << compressedData << endl;
    inputFile.close();
}



//DEBUGGING FUNCTIONS
void HuffmanTree::PrintCodes()
{
    map<char, string> codes;
    generateCodes(this->root, "", codes);

    // output the codes

    cout << "Huffman Codes: " << endl;
    for (const auto &pair : codes)
    {
        cout << pair.first << ": " << pair.second << endl;
    }
}
void HuffmanTree::PrintFrequency()
{
    cout << "Frequency of letters: " << endl;
    for (const auto &pair : letterFrequency)
    {
        if (pair.first == '\n')
        {
            cout << "\\n"
                 << ": " << pair.second << endl;
        }
        else if (pair.first == ' ')
        {
            cout << "\\s"
                 << ": " << pair.second << endl;
        }
        else
            cout << pair.first << ": " << pair.second << endl;
    }
}

HuffmanTree::~HuffmanTree() {};
