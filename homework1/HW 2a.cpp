#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int x) {
        data = x;
        left = NULL;
        right = NULL;
    }
};

class BST {
public:
    Node* root;

    BST() {
        root = NULL;
    }

    Node* insertNode(Node* node, int x) {
        if (node == NULL) {
            Node* temp = new Node(x);
            return temp;
        }

        if (x < node->data) {
            node->left = insertNode(node->left, x);
        }
        else {
            node->right = insertNode(node->right, x);
        }

        return node;
    }

    void insert(int x) {
        root = insertNode(root, x);
    }

    int getHeight(Node* node) {
        if (node == NULL) {
            return 0;
        }

        int leftH = getHeight(node->left);
        int rightH = getHeight(node->right);

        if (leftH > rightH) {
            return leftH + 1;
        }
        else {
            return rightH + 1;
        }
    }

    int height() {
        return getHeight(root);
    }
};

int main() {
    srand(time(NULL));

    cout << "n\tHeight\tHeight/log2(n)" << endl;

    for (int n = 100; n <= 10000; n += 500) {
        BST tree;

        vector<int> arr;

        for (int i = 0; i < n; i++) {
            int num = rand();
            arr.push_back(num);
        }

        for (int i = 0; i < arr.size(); i++) {
            tree.insert(arr[i]);
        }

        int h1 = tree.height();
        int h2 = tree.height();
        int h3 = tree.height();

        int h = h3;

        double ans = h / log2((double)n);

        cout << n << "\t" << h << "\t" << ans << endl;
    }

    return 0;
}
