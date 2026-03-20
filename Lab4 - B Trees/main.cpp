#include <iostream>
using namespace std;

class BTreeNode {
public:
    int *keys;      // array of keys
    int t;          // minimum degree
    BTreeNode **C;  // child pointers
    int n;          // current number of keys
    bool leaf;      // is true when node is leaf

    BTreeNode(int _t, bool _leaf) {
        t = _t;
        leaf = _leaf;

        keys = new int[2 * t - 1];
        C = new BTreeNode *[2 * t];

        n = 0;
    }


    BTreeNode* search(int k, int &idx) {
        int i = 0;
        while (i < n && k > keys[i])
            i++;

        if (i < n && keys[i] == k) {
            idx = i;
            return this;
        }

        if (leaf)
            return nullptr;

        return C[i]->search(k, idx);
    }


    void splitChild(int i, BTreeNode *y) {
        BTreeNode *z = new BTreeNode(y->t, y->leaf);
        z->n = t - 1;

        // Copy last (t-1) keys
        for (int j = 0; j < t - 1; j++)
            z->keys[j] = y->keys[j + t];

        // Copy children if not leaf
        if (!y->leaf) {
            for (int j = 0; j < t; j++)
                z->C[j] = y->C[j + t];
        }

        y->n = t - 1;

        // Shift children of current node
        for (int j = n; j >= i + 1; j--)
            C[j + 1] = C[j];

        C[i + 1] = z;

        // Shift keys
        for (int j = n - 1; j >= i; j--)
            keys[j + 1] = keys[j];

        // Middle key moves up
        keys[i] = y->keys[t - 1];

        n++;
    }


    void insertNonFull(int k) {
        int i = n - 1;

        if (leaf) {
            // Shift keys to insert
            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = k;
            n++;
        } else {
            while (i >= 0 && keys[i] > k)
                i--;

            i++;

            if (C[i]->n == 2 * t - 1) {
                splitChild(i, C[i]);

                if (keys[i] < k)
                    i++;
            }

            C[i]->insertNonFull(k);
        }
    }

    // Traversal
    void traverse() {
        int i;
        for (i = 0; i < n; i++) {
            if (!leaf)
                C[i]->traverse();
            cout << keys[i] << " ";
        }

        if (!leaf)
            C[i]->traverse();
    }


    // Find index of first key >= k
    int findKey(int k) {
        int idx = 0;
        while (idx < n && keys[idx] < k)
            ++idx;
        return idx;
    }

    void remove(int k) {
        int idx = findKey(k);

        // Case 1: key found in this node
        if (idx < n && keys[idx] == k) {
            if (leaf)
                removeFromLeaf(idx);
            else
                removeFromNonLeaf(idx);
        }
        else {
            // If leaf → key not found
            if (leaf) {
                cout << "Key not found\n";
                return;
            }

            bool flag = (idx == n);

            // Ensure child has at least t keys
            if (C[idx]->n < t)
                fill(idx);

            if (flag && idx > n)
                C[idx - 1]->remove(k);
            else
                C[idx]->remove(k);
        }
    }
    void removeFromLeaf(int idx) {
        for (int i = idx + 1; i < n; ++i)
            keys[i - 1] = keys[i];

        n--;
    }
    void removeFromNonLeaf(int idx) {
        int k = keys[idx];

        // Case 2a: predecessor has >= t keys
        if (C[idx]->n >= t) {
            int pred = getPred(idx);
            keys[idx] = pred;
            C[idx]->remove(pred);
        }
        // Case 2b: successor has >= t keys
        else if (C[idx + 1]->n >= t) {
            int succ = getSucc(idx);
            keys[idx] = succ;
            C[idx + 1]->remove(succ);
        }
        // Case 2c: merge
        else {
            merge(idx);
            C[idx]->remove(k);
        }
    }
    int getPred(int idx) {
        BTreeNode *cur = C[idx];
        while (!cur->leaf)
            cur = cur->C[cur->n];
        return cur->keys[cur->n - 1];
    }

    int getSucc(int idx) {
        BTreeNode *cur = C[idx + 1];
        while (!cur->leaf)
            cur = cur->C[0];
        return cur->keys[0];
    }
    void fill(int idx) {
        if (idx != 0 && C[idx - 1]->n >= t)
            borrowFromPrev(idx);
        else if (idx != n && C[idx + 1]->n >= t)
            borrowFromNext(idx);
        else {
            if (idx != n)
                merge(idx);
            else
                merge(idx - 1);
        }
    }
    void borrowFromPrev(int idx) {
        BTreeNode *child = C[idx];
        BTreeNode *sibling = C[idx - 1];

        for (int i = child->n - 1; i >= 0; --i)
            child->keys[i + 1] = child->keys[i];

        if (!child->leaf) {
            for (int i = child->n; i >= 0; --i)
                child->C[i + 1] = child->C[i];
        }

        child->keys[0] = keys[idx - 1];

        if (!child->leaf)
            child->C[0] = sibling->C[sibling->n];

        keys[idx - 1] = sibling->keys[sibling->n - 1];

        child->n += 1;
        sibling->n -= 1;
    }
    void borrowFromNext(int idx) {
        BTreeNode *child = C[idx];
        BTreeNode *sibling = C[idx + 1];

        child->keys[child->n] = keys[idx];

        if (!child->leaf)
            child->C[child->n + 1] = sibling->C[0];

        keys[idx] = sibling->keys[0];

        for (int i = 1; i < sibling->n; ++i)
            sibling->keys[i - 1] = sibling->keys[i];

        if (!sibling->leaf) {
            for (int i = 1; i <= sibling->n; ++i)
                sibling->C[i - 1] = sibling->C[i];
        }

        child->n += 1;
        sibling->n -= 1;
    }


    void merge(int idx) {
        BTreeNode *child = C[idx];
        BTreeNode *sibling = C[idx + 1];

        child->keys[t - 1] = keys[idx];

        for (int i = 0; i < sibling->n; ++i)
            child->keys[i + t] = sibling->keys[i];

        if (!child->leaf) {
            for (int i = 0; i <= sibling->n; ++i)
                child->C[i + t] = sibling->C[i];
        }

        for (int i = idx + 1; i < n; ++i)
            keys[i - 1] = keys[i];

        for (int i = idx + 2; i <= n; ++i)
            C[i - 1] = C[i];

        child->n += sibling->n + 1;
        n--;

        delete sibling;
    }
};

class BTree {
public:
    BTreeNode *root;
    int t;


    BTree(int _t) {
        root = nullptr;
        t = _t;
    }


    void search(int k) {
        if (!root) {
            cout << "Tree empty\n";
            return;
        }

        int idx = -1;
        BTreeNode *res = root->search(k, idx);

        if (res)
            cout << "Found key at index " << idx << endl;
        else
            cout << "Key not found\n";
    }


    void insert(int k) {
        if (!root) {
            root = new BTreeNode(t, true);
            root->keys[0] = k;
            root->n = 1;
            return;
        }

        if (root->n == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(t, false);

            s->C[0] = root;
            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < k)
                i++;

            s->C[i]->insertNonFull(k);

            root = s;
        } else {
            root->insertNonFull(k);
        }
    }

    void traverse() {
        if (root)
            root->traverse();
        cout << endl;
    }


    void remove(int k) {
        if (!root) {
            cout << "Tree empty\n";
            return;
        }

        root->remove(k);


        if (root->n == 0) {
            BTreeNode *tmp = root;

            if (root->leaf)
                root = nullptr;
            else
                root = root->C[0];

            delete tmp;
        }
    }
};

int main() {
    BTree t(3); // minimum degree = 3

    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);

    cout << "Traversal: ";
    t.traverse();
    cout<<"Seacrh for 6: "; t.search(6);
    cout<<"Search for 15: ";t.search(15);
    cout<<"Remove 6: ";
    t.remove(6);
    t.traverse();
    return 0;
}