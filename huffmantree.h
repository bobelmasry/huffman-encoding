#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <map>
#include <queue>
using namespace std;

class HuffmanTree
{
private:
    struct letter
    {
        char ch;
        int frequency;
        letter *left;
        letter *right; // used to make the tree

        letter(char c, int freq) : ch(c), frequency(freq), left(nullptr), right(nullptr)
        {
        }
        letter(char c) : ch(c), frequency(0), left(nullptr), right(nullptr)
        {
        }
        bool operator<(const letter &other) const
        {
            return frequency > other.frequency; // bigger than as max heap
        }
    };

    letter *root; // root of the tree
    map<char, int> letterFrequency;
    string line;
    priority_queue<letter> pq;

public:
    HuffmanTree();
    HuffmanTree(const HuffmanTree &);
    ~HuffmanTree();
    void getFrequency(const string &);     // get the frequency of all letters and store them in a map
    void buildHuffmanTree(); // transforms the priority queue into a tree
    void ReBuildHuffmanTree(const map<string, char> &); // transforms the laod file into a tree
    void generateCodeMap(letter *, string, map<char, string> &);
    void Zip( string &);
    void EncodeInput(const string &);
    void SaveCompressedFile(const string &,const string &);    
    void UnZip(const string &);
    void DecodeCompressedFile(const string &, const string &);
    // DEBUGGING FUNCTIONS
    void PrintFrequency();
    void PrintCodes();
   
};

#endif // HUFFMANTREE_H
