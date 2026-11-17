#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        explicit Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t count = 0;

public:
    DoublyLinkedList() = default;
    ~DoublyLinkedList() { clear(); }

    // Чтобы копирование не привело к двойному удалению узлов.
    DoublyLinkedList(const DoublyLinkedList& other) {
        for (Node* p = other.head; p != nullptr; p = p->next)
            push_back(p->data);
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            DoublyLinkedList copy(other);
            std::swap(head, copy.head);
            std::swap(tail, copy.tail);
            std::swap(count, copy.count);
        }
        return *this;
    }

    void push_front(const T& value) {
        Node* node = new Node(value);
        node->next = head;
        if (head) head->prev = node;
        else tail = node;
        head = node;
        ++count;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        node->prev = tail;
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        ++count;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("List is empty");
        Node* old = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete old;
        --count;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("List is empty");
        Node* old = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete old;
        --count;
    }

    bool remove(const T& value) {
        for (Node* p = head; p != nullptr; p = p->next) {
            if (p->data == value) {
                if (p->prev) p->prev->next = p->next;
                else head = p->next;
                if (p->next) p->next->prev = p->prev;
                else tail = p->prev;
                delete p;
                --count;
                return true;
            }
        }
        return false;
    }

    bool contains(const T& value) const {
        for (Node* p = head; p != nullptr; p = p->next)
            if (p->data == value) return true;
        return false;
    }

    vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(count);
        for (Node* p = head; p != nullptr; p = p->next)
            result.push_back(p->data);
        return result;
    }

    void print() const {
        std::cout << "[";
        for (Node* p = head; p != nullptr; p = p->next) {
            std::cout << p->data;
            if (p->next) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    void printReverse() const {
        std::cout << "[";
        for (Node* p = tail; p != nullptr; p = p->prev) {
            std::cout << p->data;
            if (p->prev) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    size_t size() const { return count; }
    bool empty() const { return head == nullptr; }

    void clear() {
        while (!empty()) pop_front();
    }
};

int main() {
    DoublyLinkedList<int> list;
    for (int value : {10, 20, 30, 40, 50}) list.push_back(value);

    std::cout << "Original list: ";
    list.print();
    std::cout << "Reverse traversal: ";
    list.printReverse();

    std::vector<int> v = list.toVector();
    std::cout << "Vector: [";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]\n";

    list.pop_back();
    list.push_front(5);
    list.remove(30);
    std::cout << "After operations: ";
    list.print();
    std::cout << "Size: " << list.size() << "\n";
}
