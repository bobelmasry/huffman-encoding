#ifndef PQ_H
#define PQ_H
using namespace std;
#include <vector>
struct letter
{
    char ch='\0';
    int frequency;
    letter *left;
    letter *right; // used to make the tree

    letter(char c, int freq=0) : ch(c), frequency(freq), left(nullptr), right(nullptr)
    {
    }
    letter(pair<char,int> letter) : ch(letter.first), frequency(letter.second), left(nullptr), right(nullptr)
    {
    }
    bool operator>(const letter &other) const
    {
        return frequency < other.frequency; // bigger than as max heap
    }
   
};
template <typename T>
class priorityqueue
{
    vector<T*> heap;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void heapifyUp(int i);
    void heapifyDown(int i);

public:
    priorityqueue() {}

    void push(const char& letter, int freq);
    void push(T* node);

    T* pop();
    T* top() const;
    int getSize() const { return heap.size(); } // Added getSize method
    bool isEmpty() const { return heap.empty(); }
};

template class priorityqueue<letter>;
#endif