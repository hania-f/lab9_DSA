#include <iostream>
#include <stack>
using namespace std;

struct Node{
    int key;
    Node *left;
    Node *right;
    Node(int k){
        left=NULL;
        right=NULL;
        key=k;
    }
};

//1 Building the BST (using recursive insertion)

Node* insert(Node* root, int key) {
    if (root==NULL){
        return new Node(key);
    }

    if (key < root->key) {
          root->left = insert(root->left,key);
    }
    else if (key > root-> key) root->right = insert(root ->right,key);

    return root;
  
}

void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    cout<<root->key<<" ";
    inorder(root->right);
}

// 2. Predecessor and successor
void findPredSucc(Node* root, int key) {
    Node *pred = NULL, *succ = NULL;
    Node* cur = root;
    while (cur != NULL) {
        if (key < cur->key) {
            succ = cur;          // possible successor
            cur = cur->left;
        } else if (key > cur->key) {
            pred = cur;          // possible predecessor
            cur = cur->right;
        } else {
            if (cur->left) {     // max of left subtree
                Node* t = cur->left;
                while (t->right) t = t->right;
                pred = t;
            }
            if (cur->right) {    // min of right subtree
                Node* t = cur->right;
                while (t->left) t = t->left;
                succ = t;
            }
            break;
        }
    }
    cout << "Key = " << key << endl;
    cout << "Predecessor: ";
    if (pred) cout << pred->key << endl; else cout << "None" << endl;
    cout << "Successor  : ";
    if (succ) cout << succ->key << endl; else cout << "None" << endl;
}

// 3 & 4. Range query - recursive
void rangeRecursive(Node* root, int lo, int hi, int& count) {
    if (root == NULL) return;
    count++;                                   // compare key >= lo
    if (root->key >= lo) {
        rangeRecursive(root->left, lo, hi, count);
        count++;                               // compare key <= hi
        if (root->key <= hi) {
            cout << root->key << " ";
            rangeRecursive(root->right, lo, hi, count);
        }
    } else {
        rangeRecursive(root->right, lo, hi, count);
    }
}

// Range query - iterative (using a stack)
void rangeIterative(Node* root, int lo, int hi, int& count) {
    stack<Node*> st;
    Node* cur = root;
    while (cur != NULL || !st.empty()) {
        while (cur != NULL) {
            count++;                           // compare key >= lo
            if (cur->key >= lo) {
                st.push(cur);
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        if (st.empty()) break;
        Node* node = st.top();
        st.pop();
        count++;                               // compare key <= hi
        if (node->key > hi) break;             // all remaining keys are bigger
        cout << node->key << " ";
        cur = node->right;
    }
}

int main() {
    int data[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    int n = 11;

    Node* root = NULL;
    for (int i = 0; i < n; i++) root = insert(root, data[i]);

    cout << "Inorder traversal: ";
    inorder(root);
    cout << endl << endl;

    findPredSucc(root, 40);

    int lo = 30, hi = 70;
    cout << "\nRange query [" << lo << ", " << hi << "]" << endl;

    int c1 = 0;
    cout << "Recursive: ";
    rangeRecursive(root, lo, hi, c1);
    cout << "| comparisons = " << c1 << endl;

    int c2 = 0;
    cout << "Iterative: ";
    rangeIterative(root, lo, hi, c2);
    cout << "| comparisons = " << c2 << endl;

    return 0;
}