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
    while (getline(inputFile, line))
    {
        for (char ch : line)
        { 
            ch = tolower(ch);
            letterFrequency[ch]++;
        }
    }
    inputFile.close();
}

// fill the priority queue
void HuffmanTree::fillPQ()
{
    for (const auto &pair : letterFrequency)
    {
        // cout << pair.first << ": " << pair.second << endl;
        pq.push({pair.first, pair.second});
    }
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
void HuffmanTree::EncodeAndSave()
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
    while (getline(inputFile, line))
    {
        for (char ch : line)
        {
            ch = tolower(ch); // Handle case sensitivity
            compressedData += codes[ch];
        }
        // compressedData += codes['\n']; // Add newline
    }
    inputFile.close();

    // Save Huffman codes and compressed data
    SaveHuffFile(codes, compressedData);
}

void HuffmanTree::LoadAndDecode()
{
    ifstream inFile("compressed.huff", ios::binary);
    if (!inFile)
    {
        cerr << "Error opening compressed.huff for reading" << endl;
        return;
    }

    // Load Huffman codes
    uint16_t codeCount;
    inFile.read(reinterpret_cast<char *>(&codeCount), sizeof(codeCount)); // Read number of codes

    map<string, char> reverseCodes; // Reverse mapping for decoding
    for (uint16_t i = 0; i < codeCount; ++i)
    {
        char ch;
        inFile.read(&ch, sizeof(ch)); // Read character
        uint16_t codeLength;
        inFile.read(reinterpret_cast<char *>(&codeLength), sizeof(codeLength)); // Read code length
        string code(codeLength, '\0');
        inFile.read(&code[0], codeLength); // Read code
        reverseCodes[code] = ch;
    }

    // Load compressed data (in bytes)
    uint32_t dataSize;
    inFile.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize)); // Read data size
    string compressedData(dataSize, '\0');
    inFile.read(&compressedData[0], dataSize); // Read compressed data
    inFile.close();

    // Decode the compressed data
    ofstream outFile("Unzipped.txt");
    if (!outFile)
    {
        cerr << "Error opening Unzipped.txt for writing" << endl;
        return;
    }

    // Convert the byte data into a bitstream (string of bits)
    string bitstream;
    for (char byte : compressedData)
    {
        bitset<8> bits(byte); // Convert each byte to a string of 8 bits
        bitstream += bits.to_string();
    }

    // Decode using the reverse Huffman codes
    string currentCode;
    for (char bit : bitstream)
    {
        currentCode += bit; // Build the current code from bits
        if (reverseCodes.find(currentCode) != reverseCodes.end()) // Match with a code
        {
            outFile << reverseCodes[currentCode]; // Write the decoded character to file
            currentCode.clear(); // Clear the current code for the next symbol
        }
    }

    outFile.close();
    cout << "Decompressed data saved to Unzipped.txt" << endl;
}

void HuffmanTree::SaveHuffFile(const map<char, string> &codes, const string &compressedData)
{
    ofstream outFile("compressed.huff", ios::binary);
    if (!outFile)
    {
        cerr << "Error opening file for writing: compressed.huff" << endl;
        return;
    }

    // Convert each Huffman code to binary and pack it into bytes
    string bitstream;
    for (const auto &pair : codes)
    {
        // Add each Huffman code to the bitstream
        bitstream += pair.second;
    }

    // Convert compressedData to bitstream as well
    bitstream += compressedData;

    // Ensure the bitstream size is a multiple of 8 (by padding with zeros if needed)
    size_t paddingBits = 8 - (bitstream.size() % 8);
    if (paddingBits != 8)
    {
        bitstream.append(paddingBits, '0');
    }

    // Now write the bitstream to the file in bytes
    for (size_t i = 0; i < bitstream.size(); i += 8)
    {
        string byteStr = bitstream.substr(i, 8);  // Get 8 bits
        unsigned char byte = 0;
        for (size_t j = 0; j < 8; ++j)
        {
            byte = (byte << 1) | (byteStr[j] - '0');  // Convert string '0'/'1' to byte
        }
        outFile.put(byte);  // Write the byte to the file
    }

    outFile.close();
    cout << "Huffman codes and compressed data saved to compressed.huff" << endl;
}

void HuffmanTree::printCodes()
{
    map<char, string> codes;
    generateCodes(this->root, "", codes);

    // output the codes

    cout << "huffman codes: " << endl;
    for (const auto &pair : codes)
    {
        cout << pair.first << ": " << pair.second << endl;
    }
}
void HuffmanTree::outputfreq()
{
    ofstream outputFile("outputFrequency.txt");
    if (outputFile)
    {
        for (const auto &pair : letterFrequency)
        {
            outputFile << pair.first << ": " << pair.second << endl;
        }
        outputFile.close();
    }
    else
    {
        cout << "Error opening output file" << endl;
    }
}
HuffmanTree::~HuffmanTree() {};
