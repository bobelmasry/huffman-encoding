#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include "huffmantree.h"
#include <cstdint>

using namespace std;

HuffmanTree::HuffmanTree() {};
HuffmanTree::HuffmanTree(const HuffmanTree &other)
{
    letterFrequency = other.letterFrequency;
    line = other.line;
    pq = other.pq;
};
// get letter frequencies
void HuffmanTree::output()
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
    std::ifstream inputFile("inputText.txt");
    if (!inputFile)
    {
        std::cerr << "Error opening inputText.txt for reading" << std::endl;
        return;
    }

    std::string compressedData;
    std::string line;
    while (getline(inputFile, line))
    {
        for (char ch : line)
        {
            ch = tolower(ch); // Handle case sensitivity
            compressedData += codes[ch];
        }
        // compressedData += codes['\n']; // Add newline
    }
    cout << "Length: " << compressedData.length() << endl;
    inputFile.close();

    // Save Huffman codes and compressed data
    SaveHuffFile(codes, compressedData);
}

void HuffmanTree::LoadAndDecode()
{
    std::ifstream inFile("compressed.huff", std::ios::binary);
    if (!inFile)
    {
        std::cerr << "``Error opening compressed.huff for reading" << std::endl;
        return;
    }

    // Load Huffman codes
    uint16_t codeCount;
    inFile.read(reinterpret_cast<char *>(&codeCount), sizeof(codeCount)); // Read number of codes

    std::map<std::string, char> reverseCodes; // Reverse mapping for decoding
    for (uint16_t i = 0; i < codeCount; ++i)
    {
        char ch;
        inFile.read(&ch, sizeof(ch)); // Read character
        uint16_t codeLength;
        inFile.read(reinterpret_cast<char *>(&codeLength), sizeof(codeLength)); // Read code length
        std::string code(codeLength, '\0');
        inFile.read(&code[0], codeLength); // Read code
        reverseCodes[code] = ch;
    }

    // Load compressed data
    uint32_t dataSize;
    inFile.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize)); // Read data size
    std::string compressedData(dataSize, '\0');
    inFile.read(&compressedData[0], dataSize); // Read compressed data
    inFile.close();

    // Decode the compressed data
    std::ofstream outFile("Unzipped.txt");
    if (!outFile)
    {
        std::cerr << "Error opening Unzipped.txt for writing" << std::endl;
        return;
    }

    std::string currentCode;
    for (char bit : compressedData)
    {
        currentCode += bit;
        if (reverseCodes.find(currentCode) != reverseCodes.end())
        {
            outFile << reverseCodes[currentCode];
            currentCode.clear();
        }
    }

    outFile.close();
    std::cout << "Decompressed data saved to Unzipped.txt" << std::endl;
}

void HuffmanTree::SaveHuffFile(const std::map<char, std::string> &codes, const std::string &compressedData)
{
    std::ofstream outFile("compressed.txt", std::ios::binary);
    if (!outFile)
    {
        std::cerr << "Error opening file for writing: compressed.huff" << std::endl;
        return;
    }

    for (const auto &pair : codes)
    {
        cout << pair.first << ": " << pair.second.size() << endl;
        outFile << pair.first << pair.second ;
    }
    outFile << " "<< compressedData;

    // Save Huffman codes
    outFile.close();
    std::cout << "Huffman codes and compressed data saved to compressed.huff" << std::endl;
}

HuffmanTree::~HuffmanTree() {};
