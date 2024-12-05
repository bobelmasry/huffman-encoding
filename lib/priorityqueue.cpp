#include "priorityqueue.h"
#include <iostream>

using namespace std;


template <typename T>

void priorityqueue<T>::heapifyUp(int i) {
   if (i!=0)
   {
       if (heap[i]>heap[parent(i)]) {
           swap(heap[i],heap[parent(i)]);
           i = parent(i);
           heapifyUp(i);
       }

   }

}

template <typename T>

void priorityqueue<T>::heapifyDown(int i) {
    int l = left(i);
    int r = right(i);
    int largest = i;


    if (l < heap.size() && heap[l]> heap[largest]) {
        largest = l;
    }


    if (r < heap.size() && heap[r]> heap[largest]) {
        largest = r;
    }


    if (largest != i) {
        std::swap(heap[i], heap[largest]);
        heapifyDown(largest);
    }
}

template <typename T>

void priorityqueue<T>::push(const char& letter, int freq) {
     heap.push_back(new T(letter, freq));
    heapifyUp(heap.size()-1);
}
template <typename T>

void priorityqueue<T>::push(T* letter) {
     heap.push_back(letter);
    heapifyUp(heap.size()-1);
}
template <typename T>

T* priorityqueue<T>::pop() {
    
    if(isEmpty()) {
        throw runtime_error("Heap underflow");
    }
    else {
        T* max =heap[0];
        swap(heap[0], heap[heap.size()-1]);
        heap.pop_back();

        heapifyDown(0);

        return max;}

}

template <typename T>

T* priorityqueue<T>::top() const {
   if(isEmpty()) {
        throw runtime_error("Heap underflow");
    }
    else {
        T* max =heap[0];
    
        return max;
    }
}