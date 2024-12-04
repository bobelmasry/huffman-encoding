#include <map>
#include <queue>
#include "huffmantree.h"
#include "QString"
#include <bitset>
#include <QFile>
#include <QDebug>
using namespace std;

HuffmanTree::HuffmanTree() {};

HuffmanTree::HuffmanTree(const HuffmanTree &other)
{
    letterFrequency = other.letterFrequency;
    line = other.line;
    pq = other.pq;
};

void HuffmanTree::getFrequency(const QString &filename)
{
    QFile inputFile(filename + ".txt");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        char ch;
        while (inputFile.read(&ch, 1) > 0)
        {
            letterFrequency[ch]++;
        }
        inputFile.close();
        // Filling the priority queue
        for (const auto &pair : letterFrequency)
        {
            pq.push({pair.first, pair.second});
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

void HuffmanTree::ReBuildHuffmanTree(const QMap<QString, QChar> &reverseCodes)
{
    root = new letter('\0');
    for (const auto &[code, ch] : reverseCodes)
    {
        letter *current = root;
        for (QChar bit : code)
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

void HuffmanTree::generateCodeMap(letter *node, QString code, map<char, QString> &codes)
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

void HuffmanTree::EncodeInput(const QString &filename)
{
    map<char, QString> codes;
    generateCodeMap(this->root, "", codes);
    QString compressedData;
    // Read the input text and encode it
    QFile inputFile(filename + ".txt");
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        char ch;
        while (inputFile.read(&ch, 1) > 0)
        {
            compressedData += codes[ch];
        };
        inputFile.close();
    }
    else
    {
        qDebug() << "Error opening"<< filename+".txt"  <<" for reading \n";
        return;
    }
}

void WriteBinaryString(ofstream &outputFile, const QString &binaryString)
{
    int padding = (8 - binaryString.size() % 8) % 8; // calculating padding
    QString paddedBinary = binaryString + QString(padding, '0');

    // add padding at the start of file
    outputFile.put(padding);

    for (size_t i = 0; i < paddedBinary.size(); i += 8) {
        bitset<8> byte(paddedBinary.substr(i, 8));
        outputFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }
}

void HuffmanTree::SaveCompressedFile(const QString &inputFilename, const QString &outputFilename)
{
    map<char, QString> codes;
    QString compressedData;
    generateCodeMap(this->root, "", codes);

    QFile inputFile(inputFilename + ".txt");
    if (!inputFile.open())
    {
        qDebug() << "Error opening" << inputFilename + ".txt" << "for reading" << "\n";
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
        qDebug() << "Error opening " << outputFilename << " for writing" << "\n";
        return;
    }

    outputFile.put(static_cast<char>(padding));

    outputFile.put(static_cast<char>(codes.size()));

    for (const auto &pair : codes)
    {
        outputFile.put(pair.first);
        outputFile.put(static_cast<char>(pair.second.size())); // each code is 1 byte

        QString codeBits = pair.second;
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
    qDebug() << "Successful Zipping To: " << outputFilename << "\n";
}

void HuffmanTree::DecodeCompressedFile(const QString &inputFilename, const QString &outputFilename)
{

    QFile inputFile(inputFilename); //qt reads text files in binary mode automatically, no need to change modes
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening" << inputFilename << "for reading";
        return;
    }

    // Read the padding byte
    unsigned char padding = 0;
    if (inputFile.read(reinterpret_cast<char*>(&padding), 1) != 1)
    {
        qDebug() << "Error reading padding byte.";
        inputFile.close();
        return;
    }

    // Read the number of codes
    unsigned char codeCount = 0;
    if (inputFile.read(reinterpret_cast<char*>(&codeCount), 1) != 1)
    {
        qDebug() << "Error reading code count.";
        inputFile.close();
        return;
    }

    QMap<QString, QChar> reverseCodes;
    for (int i = 0; i < static_cast<int>(codeCount); ++i)
    {
        unsigned char ch = 0;
        if (inputFile.read(reinterpret_cast<char*>(&ch), 1) != 1)
        {
            qDebug() << "Error reading character.";
            inputFile.close();
            return;
        }

        unsigned char codeSize = 0;
        if (inputFile.read(reinterpret_cast<char*>(&codeSize), 1) != 1)
        {
            qDebug() << "Error reading code size.";
            inputFile.close();
            return;
        }

        QString codeBits;
        int bytesToRead = (codeSize + 7) / 8;  // Calculate the number of bytes required for the bits
        QByteArray codeData = inputFile.read(bytesToRead);
        if (codeData.size() != bytesToRead)
        {
            qDebug() << "Error reading Huffman code bits.";
            inputFile.close();
            return;
        }

        for (char byte : codeData)
        {
            std::bitset<8> bits(byte);
            codeBits += QString::fromStdString(bits.to_string());
        }

        // Trim the string to the actual size of the Huffman code (since the last byte may contain extra bits)
        codeBits = codeBits.left(static_cast<int>(codeSize));

        reverseCodes[codeBits] = QChar(ch);
    }

    // Build the Huffman tree from the codes
    ReBuildHuffmanTree(reverseCodes);

    // Read the compressed data
    QString binaryData;
    while (!inputFile.atEnd())
    {
        char byte;
        if (inputFile.read(&byte, 1) != 1)
        {
            qDebug() << "Error reading compressed data.";
            inputFile.close();
            return;
        }

        // Convert byte to binary string
        std::bitset<8> bits(byte);
        binaryData += QString::fromStdString(bits.to_string());
    }

    // Remove the padding bits from the end
    binaryData = binaryData.left(binaryData.size() - static_cast<int>(padding));

    inputFile.close();

    // Open the output file for writing
    QFile outputFile(outputFilename);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Error opening" << outputFilename << "for writing";
        return;
    }

    QTextStream out(&outputFile);

    // Decode the binary data using the Huffman tree
    letter *current = root;
    for (QChar bit : binaryData)
    {
        current = (bit == '0') ? current->left : current->right;

        // If a leaf node is reached, output the character
        if (!current->left && !current->right)
        {
            out << current->ch;
            current = root;
        }
    }

    outputFile.close();
    qDebug() << "Successful UnZipping of:" << inputFilename << "to:" << outputFilename;
}

void HuffmanTree::Zip(const QString &filename)
{
    getFrequency(filename);
    buildHuffmanTree();
    EncodeInput(filename);
    SaveCompressedFile(filename, filename+".huff");
}

void HuffmanTree::UnZip(const QString &filename)
{

    DecodeCompressedFile(filename , "Decompressed.txt");
}

// DEBUGGING FUNCTIONS

void HuffmanTree::PrintCodes()
{
    map<char, QString> codes;
    generateCodeMap(this->root, "", codes);

    // output the codes

    cout << "Huffman Codes: " << endl;
    for (const auto &pair : codes)
    {
        qDebug() << pair.first << ": " << pair.second << "\n";
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