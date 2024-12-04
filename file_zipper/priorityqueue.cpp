#include "priorityqueue.h"
#include <stdexcept>

template class priorityqueue<QString>;

template<typename T>
priorityqueue<T>::priorityqueue(int maxsize) : capacity(maxsize), size(0) {
    arr = new T[capacity];
}

template<typename T>
priorityqueue<T>::~priorityqueue() {
    delete[] arr;
}

template<typename T>
void priorityqueue<T>::upheap(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (arr[index] <= arr[parent]) {
            break;
        }
        std::swap(arr[index], arr[parent]);
        index = parent;
    }
}

template<typename T>
void priorityqueue<T>::doubleCapacity() {
    capacity *= 2;
    T* newArr = new T[capacity];
    for (int i = 0; i < size; ++i) {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
}

template<typename T>
void priorityqueue<T>::insert(const T& item) {
    if (size == capacity) {
        doubleCapacity();
    }
    arr[size] = item;
    upheap(size);
    ++size;
}

template<typename T>
T priorityqueue<T>::remove() {
    if (size == 0) {
        throw std::underflow_error("Priority queue is empty.");
    }
    T maxItem = arr[0];
    arr[0] = arr[size - 1];
    --size;

    int index = 0;
    while (index < size) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int largest = index;

        if (leftChild < size && arr[leftChild] > arr[largest]) {
            largest = leftChild;
        }

        if (rightChild < size && arr[rightChild] > arr[largest]) {
            largest = rightChild;
        }

        if (largest == index) {
            break;
        }

        swap(arr[index], arr[largest]);
        index = largest;
    }

    return maxItem;
}

