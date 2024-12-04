#ifndef PQ_H
#define PQ_H
#include <QString>
using namespace std;

template<typename T>
class priorityqueue
{
    T* arr;
    int capacity;
    int size;

    void upheap(int index);
    void downheap(int index);
    void doubleCapacity(); // dynamically resize the array when full

public:
    priorityqueue(int initialCapacity = 100); //making the pq 100 for default use
    ~priorityqueue();
    void insert(const T& item);
    T remove();
    T getMax() const;
    void removeElement(const T& element);
    int getSize() const;
    bool isEmpty() const;
};

#endif
