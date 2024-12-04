#include <QString>
#include "priorityqueue.h"
using namespace std;

template<typename T>
priorityqueue<T>::priorityqueue(int maxsize):capacity(maxsize+1), size(0) //max size is plus 1 because
{arr = new T[capacity];}

template<typename T>
priorityqueue<T>::~priorityqueue()
{delete [] arr;}

template<typename T>
void priorityqueue<T>::insert(const T& item)
{upheap(item);}

template<typename T>
T priorityqueue<T>::remove()
{
    if(size==0)
    {
       arr[0]=arr[arr->size()];
        upheap(size);
    }
}

template<typename T>
void priorityqueue<T>::doubleCapacity()
{}
