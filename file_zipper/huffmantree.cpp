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

void HuffmanTree::getFrequency(const string &filename)
{
    ifstream inputFile(filename + ".txt");
    if (inputFile)
    {
        char ch;
        while (inputFile.get(ch))
        {
            letterFrequency[ch]++;
        }
        inputFile.close();
        for (const auto &pair : letterFrequency)
        {
            pq.push(letter(pair.first, pair.second));
        }
    }
    else
    {
        cout << "Error opening Input File" << endl;
        return;
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

void HuffmanTree::ReBuildHuffmanTree(const map<string, char> &reverseCodes)
{
    root = new letter('\0');
    for (const auto &[code, ch] : reverseCodes)
    {
        letter *current = root;
        for (char bit : code)
        {
            if (bit == '0')
            {
                if (!current->left)
                    current->left = new letter('\0');
                current = current->left;
            }
            else
            {
                if (!current->right)
                    current->right = new letter('\0');
                current = current->right;
            }
        }
        current->ch = ch;
    }
}

void HuffmanTree::generateCodeMap(letter *node, string code, map<char, string> &codes)
{
    if (!node)
        return;

    // if node is a leaf
    if (node->ch != '\0')
    {
        codes[node->ch] = code;
    }

    // traverse left adding 0 and traversing right adding 1
    generateCodeMap(node->left, code + "0", codes);
    generateCodeMap(node->right, code + "1", codes);
}

void HuffmanTree::EncodeInput(const string &filename)
{
    map<char, string> codes;
    generateCodeMap(this->root, "", codes);
    string compressedData;
    int firstline = 2;
    // Read the input text and encode it
    ifstream inputFile(filename + ".txt");
    if (inputFile)
    {
        char ch;
        while (inputFile.get(ch))
        {
            // ch = tolower(ch); // Handle case sensitivity
            compressedData += codes[ch];
        };
        inputFile.close();
    }
    else
    {
        cout << "Error opening"<< filename+".txt"  <<" for reading" << endl;
        return;
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

void HuffmanTree::SaveCompressedFile(const string &inputFilename, const string &outputFilename)
{
    map<char, string> codes;
    string compressedData;
    generateCodeMap(this->root, "", codes);

    ifstream inputFile(inputFilename + ".txt");
    if (!inputFile)
    {
        cout << "Error opening" << inputFilename + ".txt" << " for reading" << endl;
        return;
    }
    char ch;
    while (inputFile.get(ch))
    {
        compressedData += codes[ch];
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
    cout << "Successful Zipping To: " << outputFilename << endl;
}

void HuffmanTree::DecodeCompressedFile(const string &inputFilename, const string &outputFilename)
{
    ifstream inputFile(inputFilename, ios::binary);
    if (!inputFile)
    {
        cout << "Error opening " << inputFilename << " for reading" << endl;
        return;
    }

    // Read the padding byte
    char padding;
    inputFile.get(padding);

    // Read the number of codes
    char codeCount;
    inputFile.get(codeCount);

    // Reconstruct the Huffman code map
    map<string, char> reverseCodes;
    for (int i = 0; i < static_cast<unsigned char>(codeCount); ++i)
    {
        char ch;
        inputFile.get(ch);

        char codeSize;
        inputFile.get(codeSize);

        string codeBits;
        for (int j = 0; j < static_cast<unsigned char>(codeSize); j += 8)
        {
            char byte;
            inputFile.get(byte);
            bitset<8> bits(byte);
            codeBits += bits.to_string();
        }

        reverseCodes[codeBits.substr(0, static_cast<unsigned char>(codeSize))] = ch;
    }

    // Build the Huffman tree from the codes
    ReBuildHuffmanTree(reverseCodes);

    // Read the compressed data
    string binaryData;
    while (inputFile.peek() != EOF)
    {
        char byte;
        inputFile.get(byte);
        bitset<8> bits(byte);
        binaryData += bits.to_string();
    }

    // Remove the padding bits from the end
    binaryData = binaryData.substr(0, binaryData.size() - padding);

    // Decode the binary data using the Huffman tree
    ofstream outputFile(outputFilename);
    if (!outputFile)
    {
        cout << "Error opening " << outputFilename << " for writing" << endl;
        return;
    }

    letter *current = root;
    for (char bit : binaryData)
    {
        current = (bit == '0') ? current->left : current->right;

        // If a leaf node is reached, output the character
        if (!current->left && !current->right)
        {
            outputFile.put(current->ch);
            current = root;
        }
    }

    inputFile.close();
    outputFile.close();
    cout << "Successful UnZipping of :"<< inputFilename <<"\nTo: "<< outputFilename << endl;
};

void HuffmanTree::Zip(const string &filename)
{
    getFrequency(filename);
    buildHuffmanTree();
    EncodeInput(filename);
    SaveCompressedFile(filename, filename+".huff");
}

void HuffmanTree::UnZip(const string &filename)
{
    DecodeCompressedFile(filename , "Decompressed.txt");
}

// DEBUGGING FUNCTIONS

void HuffmanTree::PrintCodes()
{
    map<char, string> codes;
    generateCodeMap(this->root, "", codes);

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
