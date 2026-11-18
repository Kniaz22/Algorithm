#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root = nullptr;

    Node* insertNode(Node* node, int key) {
        if (!node) return new Node(key);
        if (key < node->data) node->left = insertNode(node->left, key);
        else if (key > node->data) node->right = insertNode(node->right, key);
        return node;
    }

    Node* removeNode(Node* node, int key) {
        if (!node) return nullptr;
        if (key < node->data) node->left = removeNode(node->left, key);
        else if (key > node->data) node->right = removeNode(node->right, key);
        else {
            if (!node->left) {
                Node* next = node->right;
                delete node;
                return next;
            }
            if (!node->right) {
                Node* next = node->left;
                delete node;
                return next;
            }
            // Заменяем удаляемый узел минимумом правого поддерева.
            Node* successor = node->right;
            while (successor->left) successor = successor->left;
            node->data = successor->data;
            node->right = removeNode(node->right, successor->data);
        }
        return node;
    }

    void printNode(Node* node) const {
        if (!node) return;
        printNode(node->left);
        cout << node->data << ' ';
        printNode(node->right);
    }

    int heightNode(Node* node) const {
        if (!node) return 0;
        return 1 + max(heightNode(node->left), heightNode(node->right));
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BST() = default;
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;
    ~BST() { destroy(root); }

    void insert(int key) { root = insertNode(root, key); }
    void remove(int key) { root = removeNode(root, key); }
    void print() const { printNode(root); cout << '\n'; }
    int height() const { return heightNode(root); }
    bool empty() const { return root == nullptr; }

    int minValue() const {
        Node* node = root;
        while (node->left) node = node->left;
        return node->data;
    }

    int maxValue() const {
        Node* node = root;
        while (node->right) node = node->right;
        return node->data;
    }
};

int main() {
    BST tree;
    mt19937 generator(14);
    uniform_int_distribution<int> distribution(1, 99);
    vector<int> values;

    // Создаём дерево из 20 различных случайных чисел.
    while (values.size() < 20) {
        int value = distribution(generator);
        if (find(values.begin(), values.end(), value) == values.end()) {
            values.push_back(value);
            tree.insert(value);
        }
    }

    cout << "Ishodnie numbers: ";
    for (int value : values) cout << value << ' ';
    cout << "\nSimmetrichniy obhod (po vozrastaniy): ";
    tree.print();

    if (!tree.empty()) {
        cout << "min: " << tree.minValue() << '\n';
        cout << "max: " << tree.maxValue() << '\n';
    }
    cout << "hight tree (node): " << tree.height() << '\n';

    int key;
    cout << "input a number to remove: ";
    if (!(cin >> key)) {
        cout << "Error: input int number.\n";
        return 1;
    }
    tree.remove(key);
    cout << "tree after remove ";
    tree.print();
    return 0;
}
