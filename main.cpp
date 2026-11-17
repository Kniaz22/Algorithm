#include <iostream>
#include <stdexcept>
#include <string>
#include <functional>
#include <utility>
#include <cstddef>

using namespace std;

// 1. Два стека в одном динамическом массиве.
template <typename T>
class TwoStacks {
    T* data;
    int capacity;
    int leftTop;
    int rightTop;

    void resize() {
        int newCapacity = capacity * 2;
        T* next = new T[newCapacity];
        for (int i = 0; i <= leftTop; ++i) next[i] = data[i];
        int rightCount = capacity - rightTop;
        for (int i = 0; i < rightCount; ++i)
            next[newCapacity - rightCount + i] = data[rightTop + i];
        delete[] data;
        data = next;
        rightTop = newCapacity - rightCount;
        capacity = newCapacity;
    }
public:
    explicit TwoStacks(int initialCapacity = 4)
        : data(nullptr), capacity(initialCapacity > 0 ? initialCapacity : 4),
          leftTop(-1), rightTop(capacity) { data = new T[capacity]; }
    ~TwoStacks() { delete[] data; }
    TwoStacks(const TwoStacks&) = delete;
    TwoStacks& operator=(const TwoStacks&) = delete;

    void push1(const T& value) {
        if (leftTop + 1 == rightTop) resize();
        data[++leftTop] = value;
    }
    void push2(const T& value) {
        if (leftTop + 1 == rightTop) resize();
        data[--rightTop] = value;
    }
    T pop1() {
        if (isEmpty1()) throw std::underflow_error("First stack is empty");
        return data[leftTop--];
    }
    T pop2() {
        if (isEmpty2()) throw std::underflow_error("Second stack is empty");
        return data[rightTop++];
    }
    const T& top1() const {
        if (isEmpty1()) throw std::underflow_error("First stack is empty");
        return data[leftTop];
    }
    const T& top2() const {
        if (isEmpty2()) throw std::underflow_error("Second stack is empty");
        return data[rightTop];
    }
    bool isEmpty1() const { return leftTop == -1; }
    bool isEmpty2() const { return rightTop == capacity; }
    int size1() const { return leftTop + 1; }
    int size2() const { return capacity - rightTop; }
};

// 2. Очередь на связном списке с методом reverse().
template <typename T>
class Queue {
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    int count = 0;
public:
    Queue() = default;
    ~Queue() { clear(); }
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const T& value) {
        Node* node = new Node(value);
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        ++count;
    }
    T dequeue() {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        Node* old = head;
        T value = old->data;
        head = old->next;
        if (!head) tail = nullptr;
        delete old;
        --count;
        return value;
    }
    const T& front() const {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        return head->data;
    }
    void reverse() {
        Node* previous = nullptr;
        Node* current = head;
        tail = head;
        while (current) {
            Node* next = current->next;
            current->next = previous;
            previous = current;
            current = next;
        }
        head = previous;
    }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void clear() { while (!isEmpty()) dequeue(); }
    void print() const {
        std::cout << "[";
        for (Node* p = head; p; p = p->next) {
            cout << p->data;
            if (p->next) std::cout << ", ";
        }
        cout << "]\n";
    }
};

// 3. Дек на двусвязном списке, хранит текущую сумму.
// Для T должны поддерживаться + и - (например, int, double).
template <typename T>
class SumDeque {
    struct Node {
        T data;
        Node* prev;
        Node* next;
        explicit Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    int count = 0;
    T total{};
public:
    SumDeque() = default;
    ~SumDeque() { clear(); }
    SumDeque(const SumDeque&) = delete;
    SumDeque& operator=(const SumDeque&) = delete;

    void push_front(const T& value) {
        Node* node = new Node(value);
        node->next = head;
        if (head) head->prev = node;
        else tail = node;
        head = node;
        total += value;
        ++count;
    }
    void push_back(const T& value) {
        Node* node = new Node(value);
        node->prev = tail;
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        total += value;
        ++count;
    }
    T pop_front() {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        Node* old = head;
        T value = old->data;
        head = old->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        total -= value;
        delete old;
        --count;
        return value;
    }
    T pop_back() {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        Node* old = tail;
        T value = old->data;
        tail = old->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        total -= value;
        delete old;
        --count;
        return value;
    }
    const T& front() const {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        return head->data;
    }
    const T& back() const {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        return tail->data;
    }
    T getSum() const { return total; } // O(1)
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void clear() { while (!isEmpty()) pop_front(); }
};

// 4. Приоритетная очередь на двоичной куче с компаратором.
// compare(a,b) == true означает: b имеет более высокий приоритет, чем a.
template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
    T* heap;
    int count;
    int capacity;
    Compare compare;

    void resize() {
        int nextCapacity = capacity * 2;
        T* next = new T[nextCapacity];
        for (int i = 0; i < count; ++i) next[i] = heap[i];
        delete[] heap;
        heap = next;
        capacity = nextCapacity;
    }
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (!compare(heap[parent], heap[index])) break;
            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    }
    void heapifyDown(int index) {
        while (true) {
            int left = index * 2 + 1;
            int right = index * 2 + 2;
            int best = index;
            if (left < count && compare(heap[best], heap[left])) best = left;
            if (right < count && compare(heap[best], heap[right])) best = right;
            if (best == index) break;
            std::swap(heap[index], heap[best]);
            index = best;
        }
    }
public:
    explicit PriorityQueue(Compare cmp = Compare{}, int initialCapacity = 4)
        : heap(nullptr), count(0), capacity(initialCapacity > 0 ? initialCapacity : 4),
          compare(cmp) { heap = new T[capacity]; }
    ~PriorityQueue() { delete[] heap; }
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;

    void push(const T& value) {
        if (count == capacity) resize();
        heap[count] = value;
        heapifyUp(count);
        ++count;
    }
    T pop() {
        if (isEmpty()) throw std::underflow_error("Priority queue is empty");
        T result = heap[0];
        --count;
        if (count > 0) {
            heap[0] = heap[count];
            heapifyDown(0);
        }
        return result;
    }
    const T& top() const {
        if (isEmpty()) throw std::underflow_error("Priority queue is empty");
        return heap[0];
    }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Пользовательский тип и компаратор: большие priority обслуживаются раньше.
struct Task {
    string name;
    int priority;
};
struct TaskCompare {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority < b.priority;
    }
};

int main() {
    cout << "=== 1. TWO STACKS IN ONE ARRAY ===\n";
    TwoStacks<int> stacks(2);
    stacks.push1(10);
    stacks.push2(100);
    stacks.push1(20); // проверка расширения общего массива
    stacks.push2(200);
    cout << "Stack 1 top: " << stacks.top1() << ", Stack 2 top: " << stacks.top2() << "\n";
    cout << "Stack 1 pop: " << stacks.pop1() << ", Stack 2 pop: " << stacks.pop2() << "\n\n";

    cout << "=== 2. QUEUE REVERSE ===\n";
    Queue<int> q;
    q.enqueue(10); q.enqueue(20); q.enqueue(30);
    cout << "Before: "; q.print();
    q.reverse();
    cout << "After:  "; q.print();
    cout << "Dequeued: " << q.dequeue() << "\n\n";

    cout << "=== 3. DEQUE SUM O(1) ===\n";
    SumDeque<int> dq;
    dq.push_back(10); dq.push_back(20); dq.push_front(5);
    cout << "Sum: " << dq.getSum() << "\n";
    cout << "Removed from back: " << dq.pop_back() << "\n";
    cout << "Sum after removal: " << dq.getSum() << "\n\n";

    cout << "=== 4. PRIORITY QUEUE WITH COMPARATOR ===\n";
    PriorityQueue<Task, TaskCompare> tasks;
    tasks.push({"Documentation", 2});
    tasks.push({"Critical bug", 10});
    tasks.push({"Testing", 5});
    tasks.push({"Release", 7});
    while (!tasks.isEmpty()) {
        Task task = tasks.pop();
        cout << task.name << " (priority " << task.priority << ")\n";
    }

    cout << "\n=== EMPTY STRUCTURE ERROR CHECK ===\n";
    try { q.dequeue(); q.dequeue(); q.dequeue(); } // q has only two values left
    catch (const std::underflow_error& e) { cout << "Caught: " << e.what() << "\n"; }
}
