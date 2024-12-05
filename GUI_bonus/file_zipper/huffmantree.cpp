#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <map>
#include <string>
#include "huffmantree.h"
#include "priorityqueue.h"
#include <bitset>
#include <QMessageBox>
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
    QFile inputFile(QString::fromStdString(filename));
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QChar ch;
        while (!in.atEnd())
        {
            in >> ch;
            letterFrequency[ch.toLatin1()]++;
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
        QMessageBox::critical(nullptr, "Error", "No such file: " + QString::fromStdString(filename));
        return;
    }
}

void HuffmanTree::buildHuffmanTree()
{
    if (pq.getSize() == 0)
    {
        cout << "Priority queue is empty. Cannot build Huffman Tree." << endl;
        return;
    }

    while (pq.getSize() > 1)
    {
        letter *left = pq.top();
        pq.pop();
        letter *right = pq.top();
        pq.pop();

        // create a combined node
        letter *combined = new letter('\0', left->frequency + right->frequency); // '\0' here is a null character
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
    // Read the input text and encode it
    QFile inputFile(QString::fromStdString(filename));
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        QChar ch;
        while (!in.atEnd())
        {
            in >> ch;
            compressedData += codes[ch.toLatin1()];
        }
        inputFile.close();
    }
    else
    {
        cout << "Error opening " << filename << " for reading" << endl;
        QMessageBox::critical(nullptr, "Error", "No such file: " + QString::fromStdString(filename));
        return;
    }
}

void WriteBinaryString(QFile &outputFile, const string &binaryString)
{
    int padding = (8 - binaryString.size() % 8) % 8; // calculating padding
    string paddedBinary = binaryString + string(padding, '0');

    QDataStream out(&outputFile);
    out << static_cast<char>(padding);

    for (size_t i = 0; i < paddedBinary.size(); i += 8)
    {
        bitset<8> byte(paddedBinary.substr(i, 8));
        out << static_cast<unsigned char>(byte.to_ulong());
    }
}

void HuffmanTree::SaveCompressedFile(const string &inputFilename, const string &outputFilename)
{
    map<char, string> codes;
    string compressedData;
    generateCodeMap(this->root, "", codes);

    QFile inputFile(QString::fromStdString(inputFilename));
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Error opening " << inputFilename << " for reading" << endl;
        QMessageBox::critical(nullptr, "Error", "No such file: " + QString::fromStdString(inputFilename));
        return;
    }
    QTextStream in(&inputFile);
    QChar ch;
    while (!in.atEnd())
    {
        in >> ch;
        compressedData += codes[ch.toLatin1()];
    }
    inputFile.close();

    // Calculate padding
    int padding = (8 - compressedData.size() % 8) % 8;
    string paddedBinary = compressedData + string(padding, '0'); // adding padding

    QFile outputFile(QString::fromStdString(outputFilename));
    if (!outputFile.open(QIODevice::WriteOnly))
    {
        cout << "Error opening " << outputFilename << " for writing" << endl;
        QMessageBox::critical(nullptr, "Error", "No such file: " + QString::fromStdString(outputFilename));
        return;
    }

    QDataStream out(&outputFile);
    out << static_cast<char>(padding);
    out << static_cast<char>(codes.size());

    for (const auto &pair : codes)
    {
        out << pair.first;
        out << static_cast<char>(pair.second.size());

        string codeBits = pair.second;
        while (codeBits.size() % 8 != 0)
            codeBits += "0"; // padding for the code

        for (size_t i = 0; i < codeBits.size(); i += 8)
        {
            bitset<8> byte(codeBits.substr(i, 8));
            out << static_cast<unsigned char>(byte.to_ulong());
        }
    }

    // save data as binary
    for (size_t i = 0; i < paddedBinary.size(); i += 8)
    {
        bitset<8> byte(paddedBinary.substr(i, 8));
        out << static_cast<unsigned char>(byte.to_ulong());
    }

    outputFile.close();
    cout << "\033[32mSuccessful Zipping To: " << outputFilename << endl;
    QMessageBox::information(nullptr, "Success", "Successfully zipped to: " + QString::fromStdString(outputFilename));
}


void HuffmanTree::DecodeCompressedFile(const string &inputFilename, const string &outputFilename)
{
    QFile inputFile(QString::fromStdString(inputFilename));
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        cout << "Error opening " << inputFilename << " for reading" << endl;
        QMessageBox::critical(nullptr, "Error", "No such file: " + QString::fromStdString(inputFilename));
        return;
    }

    QDataStream in(&inputFile);

    // Read the padding byte
    char padding;
    in >> padding;

    // Read the number of codes
    char codeCount;
    in >> codeCount;

    // Reconstruct the Huffman code map
    map<string, char> reverseCodes;
    for (int i = 0; i < static_cast<unsigned char>(codeCount); ++i)
    {
        char ch;
        in >> ch;

        char codeSize;
        in >> codeSize;

        string codeBits;
        for (int j = 0; j < static_cast<unsigned char>(codeSize); j += 8)
        {
            char byte;
            in >> byte;
            bitset<8> bits(byte);
            codeBits += bits.to_string();
        }

        reverseCodes[codeBits.substr(0, static_cast<unsigned char>(codeSize))] = ch;
    }

    // Build the Huffman tree from the codes
    ReBuildHuffmanTree(reverseCodes);

    // Read the compressed data
    string binaryData;
    while (!in.atEnd())
    {
        char byte;
        in >> byte;
        bitset<8> bits(byte);
        binaryData += bits.to_string();
    }

    // Remove the padding bits from the end
    binaryData = binaryData.substr(0, binaryData.size() - padding);

    // Decode the binary data using the Huffman tree
    QFile outputFile(QString::fromStdString(outputFilename));
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "Error opening " << outputFilename << " for writing" << endl;
        QMessageBox::critical(nullptr, "Error", "No such file: " + QString::fromStdString(outputFilename));
        return;
    }
    QTextStream out(&outputFile);

    letter *current = root;
    for (char bit : binaryData)
    {
        current = (bit == '0') ? current->left : current->right;

        // If a leaf node is reached, output the character
        if (!current->left && !current->right)
        {
            out << current->ch;
            current = root;
        }
    }

    inputFile.close();
    outputFile.close();
    cout << "\033[32mSuccessful UnZipping of :" << inputFilename << "\nTo: " << outputFilename << endl;
    QMessageBox::information(nullptr, "Success", "Successfully unzipped to: " + QString::fromStdString(outputFilename));
}

void HuffmanTree::Zip(string filename)
{
    getFrequency(filename);
    if (letterFrequency.empty())
    {
        cout << "Frequency map is empty. Cannot proceed with compression." << endl;
        return;
    }
    buildHuffmanTree();
    if (!root)
    {
        cout << "Huffman tree is not built. Cannot proceed with compression." << endl;
        return;
    }
    EncodeInput(filename);
    SaveCompressedFile(filename, filename + ".huff");
}

void HuffmanTree::UnZip(const string &filename)
{
    DecodeCompressedFile(filename, "Decompressed.txt");
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
