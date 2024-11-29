#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include "huffmantree.h"
#include <string>
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
    string compressedData;
    bool firstline = 1;
    generateCodes(this->root, "", codes);
    // Read the input text and encode it
    ifstream inputFile("inputText.txt");
    if (!inputFile)
    {
        cout << "Error opening inputText.txt for reading" << endl;
        return;
    }

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
    inputFile.close();
    cout << "Compressed data: " << compressedData << endl;
}

void Zip()
{
    //TODO
    // Get the compressed data and Huffman Tree THEN write it to a file USING BITS (Which is difficult)

}
void UnZip()
{
    //TODO
    // Decode the compressed data and Huffman Tree THEN write it to a file USING BITS (Which is difficult)
    
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

void WriteBinaryString(ofstream &outputFile, const string &binaryString)
{
    int padding = (8 - binaryString.size() % 8) % 8; // calculating padding
    string paddedBinary = binaryString + string(padding, '0');

    // add padding at the start of file
    outputFile.put(padding);

    for (size_t i = 0; i < paddedBinary.size(); i += 8)
    {
        bitset<8> byte(paddedBinary.substr(i, 8));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }
}

void HuffmanTree::SaveCompressedFile(const string &outputFilename)
{
    map<char, string> codes;
    string compressedData;
    generateCodes(this->root, "", codes);

    ifstream inputFile("inputText.txt");
    if (!inputFile)
    {
        cout << "Error opening inputText.txt for reading" << endl;
        return;
    }

    while (getline(inputFile, line))
    {
        for (char ch : line)
        {
            ch = tolower(ch);
            compressedData += codes[ch];
        }
        compressedData += codes['\n']; // adding a newline
    }
    inputFile.close();

    // Calculate padding
    int padding = (8 - compressedData.size() % 8) % 8;
    string paddedBinary = compressedData + string(padding, '0'); // adding padding

    ofstream outputFile(outputFilename, ios::binary);
    if (!outputFile)
    {
        cout << "Error opening " << outputFilename << " for writing" << endl;
        return;
    }

    outputFile.put(static_cast<char>(padding));

    outputFile.put(static_cast<char>(codes.size()));

    for (const auto &pair : codes)
    {
        outputFile.put(pair.first);
        outputFile.put(static_cast<char>(pair.second.size())); // each code is 1 byte

        string codeBits = pair.second;
        while (codeBits.size() % 8 != 0)
            codeBits += "0"; // padding for the code

        for (size_t i = 0; i < codeBits.size(); i += 8)
        {
            bitset<8> byte(codeBits.substr(i, 8));
            outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
        }
    }

    // save data as binary
    for (size_t i = 0; i < paddedBinary.size(); i += 8)
    {
        bitset<8> byte(paddedBinary.substr(i, 8));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    outputFile.close();
    cout << "Compressed data saved to " << outputFilename << endl;
}


HuffmanTree::~HuffmanTree() = default;
