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
    ~HuffmanTree();
    HuffmanTree(const HuffmanTree &);
    void getFrequency();     // get the frequency of all letters and store them in a map
    void buildHuffmanTree(); // transforms the priority queue into a tree
    void generateCodes(letter *, string, map<char, string> &);
    void CompressFileData();
    void Zip();
    void UnZip();
    //DEBUGGING FUNCTIONS
    void PrintFrequency();
    void PrintCodes();
    
};

#endif // HUFFMANTREE_H
