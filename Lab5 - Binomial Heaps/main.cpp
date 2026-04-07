#include <iostream>
#include <climits>
using namespace std;


struct Node {

    int key;
    int degree;
    Node *child, *sibling,*p;

    Node(int k) : key(k), degree(0), p(nullptr), child(nullptr), sibling(nullptr) {}

};


class BinomialHeap {


private:
    Node* head;
    void BinomialLink(Node *y,Node *z) {
        y->p=z;
        y->sibling=z->child;
        z->child=y;
        z->degree++;

    }

    Node* BinomialHeapMerge (Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;
        Node x(0);
        Node* tail = &x;

        while (h1 && h2) {
            if (h1 -> degree <= h2 -> degree) {
                tail->sibling=h1;
                h1=h1->sibling;

            }
            else {
                tail->sibling=h2;
                h2=h2->sibling;
            }
            tail = tail->sibling;
        }
        if (h1 != nullptr) {
            tail->sibling = h1;
        } else {
            tail->sibling = h2;
        }

        return x.sibling;

    }

public:
    BinomialHeap() {
        head=nullptr;
    }

    Node* BinomialHeapMin() {
        Node *y=nullptr;
        Node *x=this->head;

        int minVal = INT_MAX;

        while (x!=nullptr) {
            if (x->key<minVal) {
                minVal=x->key;
                y=x;
            }
            x=x->sibling;
        }
     return y;
    }



    void BinomialHeapUnion(BinomialHeap &other) {
        Node* newHead = BinomialHeapMerge(this->head, other.head);
        if (!newHead) {
            this->head = nullptr;
            return;
        }

        Node* prev_x = nullptr;
        Node* x = newHead;
        Node* next_x = x->sibling;

        while (next_x) {
            // Case 1 & 2: Degrees are different or three roots have same degree
            if ((x->degree != next_x->degree) ||(next_x->sibling && next_x->sibling->degree == x->degree)) {
                prev_x = x;
                x = next_x;
                }
            // Case 3: x has smaller key, link next_x to x
            else if (x->key <= next_x->key) {
                x->sibling = next_x->sibling;
                BinomialLink(next_x, x);
            }
            // Case 4: next_x has smaller key, link x to next_x
            else {
                if (!prev_x) newHead = next_x;
                else prev_x->sibling = next_x;
                BinomialLink(x, next_x);
                x = next_x;
            }
            next_x = x->sibling;
        }
        this->head = newHead;
        other.head = nullptr; // Original heaps are destroyed
    }

    Node* insert(int key) {
        Node* newNode = new Node(key);
        BinomialHeap tempHeap;
        tempHeap.head = newNode;
        this->BinomialHeapUnion(tempHeap);
        return newNode;
    }



    int BinomialHeapExtractMin () {
        if (!head) return -1;
        //find min root and its prev node
        Node* minNode = head;
        Node* prevMin = nullptr;
        Node* curr = head;
        Node* prev = nullptr;

        while (curr) {
            if (curr->key < minNode->key) {
                minNode = curr;
                prevMin = prev;
            }
            prev = curr;
            curr = curr->sibling;
        }


        //remove minNode from root list
        if (prevMin) prevMin->sibling = minNode->sibling;
        else head = minNode->sibling;


        //reverse children list to create a new heap h'
        Node* childList = minNode->child;
        Node* prevChild = nullptr;
        while (childList) {
            Node* nextChild = childList->sibling;
            childList->sibling = prevChild;
            childList->p = nullptr;
            prevChild = childList;
            childList = nextChild;
        }

        BinomialHeap hPrime;
        hPrime.head = prevChild;

        // union current heap with h'
        this->BinomialHeapUnion(hPrime);

        int minVal = minNode->key;
        delete minNode;
        return minVal;
    }



    void BinomialHeapDecreaseKey(Node* x, int k) {
        if (k>x->key) {
            cout<<"Error: new key is greater than current key";
            return;
        }
        x->key =k;
        Node* y=x;
        Node*z=y->p;
        while (z&& y->key<z->key) {
            swap(y->key, z->key);
            y=z;
            z=y->p;
        }

    }

    void BinomialHeapDelete(Node* x) {
        BinomialHeapDecreaseKey( x, INT_MIN);
        this->BinomialHeapExtractMin();
    }

};





int main() {
    BinomialHeap H;

    cout << "--- 1. Testing Insert ---" << endl;
    cout << "Inserting: 10, 20, 30, 40, 50" << endl;

    H.insert(10);
    Node* node20 = H.insert(20);
    H.insert(30);
    Node* node40 = H.insert(40);
    H.insert(50);

    cout << "\n--- 2. Testing Minimum ---" << endl;
    Node* minNode = H.BinomialHeapMin();
    if (minNode) {
        cout << "Current Min: " << minNode->key  << endl;
    }

    cout << "\n--- 3. Testing DecreaseKey ---" << endl;
    cout << "Decreasing the node with key 40 down to 5." << endl;
    H.BinomialHeapDecreaseKey(node40, 5);

    minNode = H.BinomialHeapMin();
    if (minNode) {
        cout << "New Min after DecreaseKey: " << minNode->key  << endl;
    }

    cout << "\n--- 4. Testing ExtractMin ---" << endl;
    int extracted = H.BinomialHeapExtractMin();
    cout << "Extracted value: " << extracted  << endl;


    minNode = H.BinomialHeapMin();
    if (minNode) {
        cout << "New Min after Extraction: " << minNode->key  << endl;
    }

    cout << "\n--- 5. Testing Delete ---" << endl;
    cout << "Deleting the node with key 20." << endl;
    H.BinomialHeapDelete(node20);


    cout << "\n--- 6. Emptying the Heap to verify integrity ---" << endl;
    while ((minNode = H.BinomialHeapMin()) != nullptr) {
        cout << "Extracted: " << H.BinomialHeapExtractMin() << endl;
    }
    cout << "Heap is now empty." << endl;

    return 0;
}