#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <QString>
#include <QMap>
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
    QMap<QChar, int> letterFrequency;
    QString line;
    priority_queue<letter> pq;

public:
    HuffmanTree();
    HuffmanTree(const HuffmanTree &);
    ~HuffmanTree();
    void getFrequency(const QString &);     // get the frequency of all letters and store them in a map
    void buildHuffmanTree(); // transforms the priority queue into a tree
    void ReBuildHuffmanTree(const QMap<QString, QChar> &); // transforms the laod file into a tree
    void generateCodeMap(letter *, QString, QMap<QChar, QString> &);
    void Zip(const QString &);
    void EncodeInput(const QString &);
    void SaveCompressedFile(const QString &,const QString &);
    void UnZip(const QString &);
    void DecodeCompressedFile(const QString &, const QString &);
    // DEBUGGING FUNCTIONS
    void PrintFrequency();
    void PrintCodes();

};

#endif // HUFFMANTREE_H
