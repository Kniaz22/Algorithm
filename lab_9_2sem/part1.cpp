#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    Node* root = nullptr;

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    void preOrder(Node* node) const {
        if (!node) return;
        cout << node->data << ' ';
        preOrder(node->left);
        preOrder(node->right);
    }

    void inOrder(Node* node) const {
        if (!node) return;
        inOrder(node->left);
        cout << node->data << ' ';
        inOrder(node->right);
    }

    void postOrder(Node* node) const {
        if (!node) return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << ' ';
    }

    // Возвращает высоту поддерева и обновляет диаметр в рёбрах.
    int calculateDiameter(Node* node, int& diameter) const {
        if (!node) return 0;
        int leftHeight = calculateDiameter(node->left, diameter);
        int rightHeight = calculateDiameter(node->right, diameter);
        diameter = max(diameter, leftHeight + rightHeight);
        return 1 + max(leftHeight, rightHeight);
    }

public:
    BinaryTree() = default;
    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;
    ~BinaryTree() { destroy(root); }

    void buildSample() {
        destroy(root);
        root = new Node(10);
        root->left = new Node(5);
        root->right = new Node(15);
        root->left->left = new Node(3);
        root->left->right = new Node(7);
        root->left->right->left = new Node(6);
        root->right->right = new Node(20);
        root->right->right->left = new Node(18);
    }

    void printPreOrder() const { preOrder(root); cout << '\n'; }
    void printInOrder() const { inOrder(root); cout << '\n'; }
    void printPostOrder() const { postOrder(root); cout << '\n'; }

    void printLevelOrder() const {
        if (!root) { cout << "Дерево пустое\n"; return; }
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            cout << current->data << ' ';
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        cout << '\n';
    }

    int diameter() const {
        int result = 0;
        calculateDiameter(root, result);
        return result;
    }
};

int main() {
    BinaryTree tree;
    tree.buildSample();
    cout << "pryamoj obhod: "; tree.printPreOrder();
    cout << "Simmetrichnyj obhod: "; tree.printInOrder();
    cout << "Obratnyj obhod: "; tree.printPostOrder();
    cout << "Urovnevyj obhod: "; tree.printLevelOrder();
    cout << "Diametr dereva (kolichestvo ryober): " << tree.diameter() << '\n';
    return 0;
}
