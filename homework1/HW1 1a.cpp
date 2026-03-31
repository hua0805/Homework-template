#include <iostream>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;  //判斷是否為空
    virtual const T& Top() const = 0;  //回傳最小值
    virtual void Push(const T& x) = 0;  //插入
    virtual void Pop() = 0;  //刪除最小值
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int size;
    int capacity;

    void heapifyDown(int index) {
        int smallest = index;
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;

        if (leftChild < size && heap[leftChild] < heap[smallest]) {
            smallest = leftChild;
        }

        if (rightChild < size && heap[rightChild] < heap[smallest]) {
            smallest = rightChild;
        }

        if (smallest != index) {
            T temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            heapifyDown(smallest);
        }
    }

public:
    MinHeap(int initialCapacity = 10) {
        capacity = initialCapacity;
        size = 0;
        heap = new T[capacity];
    }

    ~MinHeap() {
        delete[] heap;
    }

    bool IsEmpty() const override {
        return size == 0;
    }

    const T& Top() const override {
        if (IsEmpty()) {
            cout << "Heap is empty!" << endl;
            exit(1);
        }
        return heap[0];
    }

    void Push(const T& x) override {
        T* newHeap = new T[capacity + 1];
        for (int i = 0; i < size; i++) {
            newHeap[i] = heap[i];
        }

        newHeap[size] = x;

        delete[] heap;
        heap = newHeap;
        capacity = capacity + 1;
        size++;

        for (int i = size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    void Pop() override {
        if (IsEmpty()) {
            return;
        }

        heap[0] = heap[size - 1];
        size--;

        for (int i = size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }
};

int main() {
    MinHeap<int> h;

    h.Push(9);
    h.Push(3);
    h.Push(-5);
    h.Push(12);
    h.Push(7);
    h.Push(2);

    cout << "Top: " << h.Top() << endl;

    h.Pop();
    cout << "Top after one pop: " << h.Top() << endl;

    h.Pop();
    cout << "Top after two pops: " << h.Top() << endl;

    return 0;
}
