#include <iostream>
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

    bool searchNode(Node* node, int key) const {
        if (!node) return false;
        if (node->data == key) return true;
        return key < node->data ? searchNode(node->left, key)
                                : searchNode(node->right, key);
    }

    Node* findMin(Node* node) const {
        while (node->left) node = node->left;
        return node;
    }

    Node* removeNode(Node* node, int key) {
        if (!node) return nullptr;
        if (key < node->data) node->left = removeNode(node->left, key);
        else if (key > node->data) node->right = removeNode(node->right, key);
        else {
            // Удаление узла без левого потомка.
            if (!node->left) {
                Node* right = node->right;
                delete node;
                return right;
            }
            // Удаление узла без правого потомка.
            if (!node->right) {
                Node* left = node->left;
                delete node;
                return left;
            }
            // При двух потомках берём минимум правого поддерева.
            Node* successor = findMin(node->right);
            node->data = successor->data;
            node->right = removeNode(node->right, successor->data);
        }
        return node;
    }

    void inOrderNode(Node* node, vector<int>& values) const {
        if (!node) return;
        inOrderNode(node->left, values);
        values.push_back(node->data);
        inOrderNode(node->right, values);
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
    bool search(int key) const { return searchNode(root, key); }
    void remove(int key) { root = removeNode(root, key); }

    vector<int> inOrder() const {
        vector<int> result;
        inOrderNode(root, result);
        return result;
    }

    void print() const {
        for (int value : inOrder()) cout << value << ' ';
        cout << '\n';
    }

    // Добавляем элементы второго дерева в первое без повторов.
    void merge(const BST& other) {
        for (int value : other.inOrder()) insert(value);
    }
};

int main() {
    BST first;
    BST second;
    for (int value : {50, 30, 70, 20, 40, 60, 80}) first.insert(value);
    for (int value : {35, 40, 65, 75, 90}) second.insert(value);

    cout << "First BST: "; first.print();
    cout << "Second BST: "; second.print();
    cout << "search  40: " << (first.search(40) ? "Find" : "not Find") << '\n';
    cout << "search  99: " << (first.search(99) ? "Find" : "not Find") << '\n';

    first.remove(20); // Удаляем лист.
    first.remove(30); // Удаляем узел с одним потомком.
    first.remove(70); // Удаляем узел с двумя потомками.
    cout << "After third remove: "; first.print();

    first.merge(second);
    cout << "After merging two BSTs: "; first.print();
    cout << "The second BST has not changed: "; second.print();
    return 0;
}
