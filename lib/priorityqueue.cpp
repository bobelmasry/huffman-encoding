#include "priorityqueue.h"



template <typename T>

void priorityqueue<T>::heapifyUp(int i) {
  while (i > 0 && *heap[i] > *heap[parent(i)]) {
        swap(heap[i], heap[parent(i)]);
        i = parent(i);
    }

}

template <typename T>

void priorityqueue<T>::heapifyDown(int i) {
     int largest = i;
    int l = left(i);
    int r = right(i);

    if (l < heap.size() && *heap[l] > *heap[largest]) {
        largest = l;
    }
    if (r < heap.size() && *heap[r] > *heap[largest]) {
        largest = r;
    }

    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapifyDown(largest);
    } 
}

template <typename T>

void priorityqueue<T>::push(const char& letter, int freq) {
    T* newElement = new T(letter,freq);
    heap.push_back(newElement);
    heapifyUp(heap.size() - 1);
}
template <typename T>

void priorityqueue<T>::push(T* letter) {
    heap.push_back(letter);
    heapifyUp(heap.size() - 1);
}
template <typename T>

T* priorityqueue<T>::pop() {
    
     if (isEmpty()) {
        throw runtime_error("Heap underflow");
    }

    T* max = heap[0];
    swap(heap[0], heap[heap.size() - 1]);
    heap.pop_back();
    heapifyDown(0);
    return max;

}

template <typename T>

T* priorityqueue<T>::top() const {
    if (isEmpty()) {
        throw runtime_error("Heap underflow");
    }
    return heap[0];
}