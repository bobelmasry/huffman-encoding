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

    long originalSize = getFileSize(inputFilename);
    ifstream inputFile(inputFilename);
    if (!inputFile) {
        cout << "Error opening " << inputFilename << " for reading" << endl;
        return;
    }
    
    string content((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // Optimize Huffman codes to use shorter bit sequences
    vector<pair<int, char>> freqList;
    for (const auto& pair : letterFrequency) {
        freqList.push_back({pair.second, pair.first});
    }
    sort(freqList.begin(), freqList.end(), greater<pair<int, char>>());

    // Reassign shorter codes to more frequent characters
    map<char, string> optimizedCodes;
    string code;
    for (size_t i = 0; i < freqList.size(); i++) {
        size_t len = 1;
        while ((1 << len) <= i + 1) len++;
        code = bitset<32>(i).to_string().substr(32-len);
        optimizedCodes[freqList[i].second] = code;
    }

    ofstream outputFile(outputFilename, ios::binary);
    if (!outputFile) {
        cout << "Error opening " << outputFilename << " for writing" << endl;
        return;
    }

    // Write table size (1 byte)
    outputFile.put(static_cast<unsigned char>(optimizedCodes.size()));

    // Write character table more efficiently
    for (const auto &pair : optimizedCodes) {
        outputFile.put(pair.first);
        outputFile.put(static_cast<unsigned char>(pair.second.length()));
    }

    // Pack data bits tightly
    unsigned char byte = 0;
    int bitsUsed = 0;
    
    for (char ch : content) {
        const string& code = optimizedCodes[ch];
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

    long compressedSize = getFileSize(outputFilename);
    double compressionRatio = (1.0 - static_cast<double>(compressedSize) / originalSize) * 100;
    
    cout << "\nCompression Statistics:" << endl;
    cout << "Original size: " << originalSize << " bytes" << endl;
    cout << "Final compressed size: " << compressedSize << " bytes" << endl;
    cout << "Compression ratio: " << compressionRatio << "%" << endl;

    cout << "\033[32mSuccessful Zipping To: " << outputFilename << endl;
}

void HuffmanTree::DecodeCompressedFile(const string &inputFilename, const string &outputFilename)
{
    ifstream inputFile(inputFilename, ios::binary);
    if (!inputFile) {
        cout << "Error opening " << inputFilename << " for reading" << endl;
        return;
    }

    // Read header
    char padding = inputFile.get();
    char codeCount = inputFile.get();

    // Reconstruct the Huffman code map
    map<string, char> reverseCodes;
    for (int i = 0; i < static_cast<unsigned char>(codeCount); ++i) {
        char ch = inputFile.get();
        char codeLen = inputFile.get();
        
        string code;
        int bytesNeeded = (codeLen + 7) / 8;  // Calculate needed bytes
        
        // Read and process each byte of the code
        for (int j = 0; j < bytesNeeded; j++) {
            bitset<8> bits(inputFile.get());
            for (int k = 7; k >= 0 && code.length() < codeLen; k--) {
                code += bits[k] ? '1' : '0';
            }
        }
        
        reverseCodes[code] = ch;
    }

    // Rebuild the Huffman tree
    ReBuildHuffmanTree(reverseCodes);

    // Read and decode the compressed data
    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cout << "Error opening " << outputFilename << " for writing" << endl;
        return;
    }

    letter *current = root;
    char byte;
    int bitsProcessed = 0;
    long totalBits = 0;
    
    // Get file size and calculate total bits
    inputFile.seekg(0, ios::end);
    long fileSize = inputFile.tellg();
    inputFile.seekg(-((fileSize - inputFile.tellg()) - 1), ios::cur);
    
    // Process each byte of compressed data
    while (inputFile.get(byte)) {
        bitset<8> bits(byte);
        int limit = (inputFile.peek() == EOF && padding > 0) ? 8 - padding : 8;
        
        for (int i = 7; i >= 8 - limit; i--) {
            current = bits[i] ? current->right : current->left;
            
            if (!current->left && !current->right) {
                outputFile.put(current->ch);
                current = root;
            }
        }
    }

    inputFile.close();
    outputFile.close();
    
    // Calculate and display compression statistics
    long originalSize = getFileSize(outputFilename);
    long compressedSize = getFileSize(inputFilename);
    double compressionRatio = (1.0 - static_cast<double>(compressedSize) / originalSize) * 100;
    
    cout << "\033[32mDecompression Statistics:" << endl;
    cout << "Original size: " << originalSize << " bytes" << endl;
    cout << "Compressed size: " << compressedSize << " bytes" << endl;
    cout << "Compression ratio: " << compressionRatio << "%" << endl;
    cout << "Successfully unzipped " << inputFilename << " to " << outputFilename << endl;
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