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
    void doubleCapacity();


public:
    priorityqueue(int maxsize);
    ~priorityqueue();
    void insert(const T& item); //T& so we do not waste memory copying
    T remove();
};


#endif
