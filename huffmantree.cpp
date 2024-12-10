#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <algorithm>
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
    ifstream inputFile(filename);
    if (inputFile)
    {
        char ch;
        while (inputFile.get(ch))
        {
            // ch = tolower(ch);
            letterFrequency[ch]++;
        }
        inputFile.close();
        // Filling the priority queue
        for (const auto &pair : letterFrequency)
        {
            pq.push(pair.first, pair.second);
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
    while (pq.getSize() > 1)
    {
       
        letter *left = pq.top();
        pq.pop();
        letter *right = pq.top();
        pq.pop();

        // create a combined node
        letter *combined = new letter('\0', left->frequency + right->frequency); //'\0' here is a null character
        combined->left = left;
        combined->right = right;

        pq.push(combined);
    }
    letter *root = pq.top();
    pq.pop();

    this->root = root;
}

void HuffmanTree::ReBuildHuffmanTree(const map<string, char> &reverseCodes)
{
    root = new letter('\0');
    for (const auto &pair : reverseCodes) {
        const string& code = pair.first;
        char ch = pair.second;
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
    ifstream inputFile(filename);
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
        cout << "Error opening" << filename << " for reading" << endl;
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
    generateCodeMap(this->root, "", codes);

    // Read input file
    ifstream inputFile(inputFilename);
    if (!inputFile) {
        cout << "Error opening " << inputFilename << " for reading" << endl;
        return;
    }
    
    string content((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    ofstream outputFile(outputFilename, ios::binary);
    if (!outputFile) {
        cout << "Error opening " << outputFilename << " for writing" << endl;
        return;
    }

    // Write table size
    unsigned char tableSize = static_cast<unsigned char>(codes.size());
    outputFile.put(tableSize);

    // Write character table
    for (const auto &pair : codes) {
        outputFile.put(pair.first);  // character
        unsigned char codeLen = static_cast<unsigned char>(pair.second.length());
        outputFile.put(codeLen);     // code length
        
        // Pack the code bits
        unsigned char byte = 0;
        int bitsUsed = 0;
        for (char bit : pair.second) {
            byte = (byte << 1) | (bit == '1');
            bitsUsed++;
            if (bitsUsed == 8) {
                outputFile.put(byte);
                byte = 0;
                bitsUsed = 0;
            }
        }
        if (bitsUsed > 0) {
            byte <<= (8 - bitsUsed);
            outputFile.put(byte);
        }
    }

    // Write encoded content
    unsigned char byte = 0;
    int bitsUsed = 0;
    for (char ch : content) {
        const string& code = codes[ch];
        for (char bit : code) {
            byte = (byte << 1) | (bit == '1');
            bitsUsed++;
            if (bitsUsed == 8) {
                outputFile.put(byte);
                byte = 0;
                bitsUsed = 0;
            }
        }
    }
    if (bitsUsed > 0) {
        byte <<= (8 - bitsUsed);
        outputFile.put(byte);
    }

    outputFile.close();

    long originalSize = getFileSize(inputFilename);
    long compressedSize = getFileSize(outputFilename);
    double compressionRatio = (1.0 - static_cast<double>(compressedSize) / originalSize) * 100;
    }

void HuffmanTree::DecodeCompressedFile(const string &inputFilename, const string &outputFilename)
{
    ifstream inputFile(inputFilename, ios::binary);
    if (!inputFile) {
        cout << "Error opening " << inputFilename << " for reading" << endl;
        return;
    }

    // Read table size
    unsigned char tableSize;
    inputFile.read(reinterpret_cast<char*>(&tableSize), 1);
    cout << "Table size read: " << (int)tableSize << endl;

    // Read and reconstruct the codes
    map<string, char> reverseCodes;
    for (int i = 0; i < tableSize; ++i) {
        char ch;
        unsigned char codeLen;
        
        inputFile.read(&ch, 1);
        inputFile.read(reinterpret_cast<char*>(&codeLen), 1);
        
        // Read the actual code bits
        string code;
        unsigned char codeByte;
        for (int j = 0; j < (codeLen + 7) / 8; j++) {
            inputFile.read(reinterpret_cast<char*>(&codeByte), 1);
            for (int k = 7; k >= 0 && code.length() < codeLen; k--) {
                code += ((codeByte >> k) & 1) ? '1' : '0';
            }
        }
        
        reverseCodes[code] = ch;
        cout << "Read code: '" << ch << "' -> " << code << endl;
    }

    cout << "Number of codes read: " << reverseCodes.size() << endl;

    // Open output file
    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cout << "Error opening " << outputFilename << " for writing" << endl;
        return;
    }

    // Decode the data
    string currentCode;
    char byte;
    int bytesRead = 0;
    while (inputFile.read(&byte, 1)) {
        bytesRead++;
        for (int i = 7; i >= 0; i--) {
            currentCode += ((byte >> i) & 1) ? '1' : '0';
            
            auto it = reverseCodes.find(currentCode);
            if (it != reverseCodes.end()) {
                outputFile.put(it->second);
                currentCode.clear();
            }
        }
    }

    cout << "Bytes read during decoding: " << bytesRead << endl;
    cout << "Final current code: " << currentCode << endl;

    inputFile.close();
    outputFile.close();
}

void HuffmanTree::Zip(string filename)
{
    getFrequency(filename + ".txt");
    buildHuffmanTree();
    EncodeInput(filename + ".txt");
    SaveCompressedFile(filename + ".txt", filename + ".huff");
}

void HuffmanTree::UnZip(const string &filename)
{
    DecodeCompressedFile(filename, "Decompressed.txt");
}

long long HuffmanTree::getFileSize(const string& fileName) {
    ifstream file(fileName, ios::binary | ios::ate);
    if (!file.is_open()) return 0;
    return file.tellg();
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