#include <iostream>
#include <string>
using namespace std;

struct Node {
    string word;
    int freq;
    Node *left, *right;
    Node(string w) {
        word = w;
        freq = 1;
        left = NULL;
        right = NULL;
    }
};

// Insert a word (or increase its frequency if it already exists)
Node* insert(Node* root, string w) {
    if (root == NULL) return new Node(w);
    if (w < root->word) root->left = insert(root->left, w);
    else if (w > root->word) root->right = insert(root->right, w);
    else root->freq++;
    return root;
}

// ---------- displayAlphabetical ----------
void displayAlphabetical(Node* root) {
    if (root == NULL) return;
    displayAlphabetical(root->left);
    cout << "  " << root->word << " : " << root->freq << endl;
    displayAlphabetical(root->right);
}

// Helper: copy all nodes into an array (in alphabetical order)
void collect(Node* root, Node* arr[], int& n) {
    if (root == NULL) return;
    collect(root->left, arr, n);
    arr[n++] = root;
    collect(root->right, arr, n);
}

// ---------- displayByFrequency ----------
void displayByFrequency(Node* root) {
    Node* arr[100];
    int n = 0;
    collect(root, arr, n);

    // bubble sort, highest frequency first
    // (equal frequencies stay in alphabetical order)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j]->freq < arr[j + 1]->freq) {
                Node* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++)
        cout << "  " << arr[i]->word << " : " << arr[i]->freq << endl;
}

// ---------- findMostFrequentWord ----------
int maxFreq(Node* root) {
    if (root == NULL) return 0;
    int m = root->freq;
    int l = maxFreq(root->left);
    int r = maxFreq(root->right);
    if (l > m) m = l;
    if (r > m) m = r;
    return m;
}

void printWithFreq(Node* root, int f) {
    if (root == NULL) return;
    printWithFreq(root->left, f);
    if (root->freq == f) cout << "  " << root->word << endl;
    printWithFreq(root->right, f);
}

void findMostFrequentWord(Node* root) {
    int f = maxFreq(root);
    cout << "  Highest frequency = " << f << endl;
    cout << "  Word(s):" << endl;
    printWithFreq(root, f);
}

// ---------- Summary ----------
int countUnique(Node* root) {
    if (root == NULL) return 0;
    return 1 + countUnique(root->left) + countUnique(root->right);
}

int countTotal(Node* root) {
    if (root == NULL) return 0;
    return root->freq + countTotal(root->left) + countTotal(root->right);
}

int main() {
    string text = "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";

    Node* root = NULL;
    string word = "";

    // Parse the paragraph: letters build a word, anything else ends it
    for (int i = 0; i <= (int)text.length(); i++) {
        char c = (i < (int)text.length()) ? text[i] : ' ';
        if (isalpha(c)) {
            word += tolower(c);          // ignore case
        } else if (word != "") {
            root = insert(root, word);
            word = "";
        }
    }

    cout << "Alphabetical order:" << endl;
    displayAlphabetical(root);

    cout << "\nBy frequency (highest to lowest):" << endl;
    displayByFrequency(root);

    cout << "\nMost frequent word:" << endl;
    findMostFrequentWord(root);

    cout << "\nSummary:" << endl;
    cout << "  Total unique words = " << countUnique(root) << endl;
    cout << "  Total word count   = " << countTotal(root) << endl;

    return 0;
}