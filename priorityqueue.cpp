#include "priorityqueue.h"
#include <stdexcept>
#include <algorithm> // For std::swap
#include <algorithm> // For std::swap

template<typename T>
priorityqueue<T>::priorityqueue(int initialCapacity)
    : capacity(initialCapacity), size(0)
{
    arr = new T[capacity];
}

template<typename T>
priorityqueue<T>::~priorityqueue()
{
    delete[] arr;
}

template<typename T>
void priorityqueue<T>::upheap(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;
        if (arr[index] <= arr[parent])
        {
            break;
        }
        swap(arr[index], arr[parent]);
        index = parent;
    }
}

template<typename T>
void priorityqueue<T>::downheap(int index)
{
    while (index < size)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int largest = index;

        if (leftChild < size && arr[leftChild] > arr[largest])
        {
            largest = leftChild;
        }

        if (rightChild < size && arr[rightChild] > arr[largest])
        {
            largest = rightChild;
        }

        if (largest == index)
        {
            break;
        }

        swap(arr[index], arr[largest]);
        index = largest;
    }
}

template<typename T>
void priorityqueue<T>::doubleCapacity()
{
    capacity *= 2;
    T* newArr = new T[capacity];
    for (int i = 0; i < size; ++i)
    {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
}

template<typename T>
void priorityqueue<T>::push(const T& item)
{
    if (size == capacity)
    {
        doubleCapacity();
    }
    arr[size] = item;
    upheap(size);
    ++size;
}

template<typename T>
T priorityqueue<T>::pop()
{
    if (size == 0)
    {
        throw std::underflow_error("Priority queue is empty");
    }

    T maxItem = arr[0];
    arr[0] = arr[size - 1];
    --size;
    downheap(0);

    return maxItem;
}

template<typename T>
void priorityqueue<T>::removeElement(const T& element)
{
    int index = -1;
    for (int i = 0; i < size; ++i)
    {
        if (arr[i] == element)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        throw invalid_argument("Element not found in the priority queue.");
    }

    swap(arr[index], arr[size - 1]);

    --size;

    if (index < size)
    {
        int parent = (index - 1) / 2;
        if (index > 0 && arr[index] > arr[parent])
        {
            upheap(index);
        }
        else
        {
            downheap(index);
        }
    }
}

template<typename T>
T priorityqueue<T>::top() const
{
    if (size == 0)
    {
        throw underflow_error("Priority queue is empty.");
    }
    return arr[0]; 
}


template<typename T>
int priorityqueue<T>::getSize() const
{
    return size;
}

template<typename T>
bool priorityqueue<T>::isEmpty() const
{
    return(size == 0) ;
}
