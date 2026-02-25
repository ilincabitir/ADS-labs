#include <iostream>
using namespace std;


struct Node {
    int key;          // key
    Node* p;          // parent
    Node* left;       // left child
    Node* right;      // right child

    Node(int k) : key(k), p(nullptr), left(nullptr), right(nullptr) {}
};

class BSTree {
public:
    Node* root;
    BSTree(): root(nullptr){}

    Node* search(int k) {
        Node*x =root;
        while (x!=nullptr && k != x->key) {
            if (k< x->key)
                x=x->left;
            else
                x=x->right;
        }
        return x;
    }

    Node* minimum(Node* x) {
        while (x->left !=nullptr)
            x=x->left;            //search left subtrees until it finds smallest value
        return x;
    }
    Node* maximum(Node* x) {
        while (x->right != nullptr)
            x = x->right;          //search right subtrees until maximum value is found
        return x;
    }
    Node   *successor(Node* x) {             //successor = the node with the next largest key after the current node
        if (x->right != nullptr)         //if node has right subtree, successor = leftmost (min) node in right subtree
            return minimum(x->right);
        Node* y= x->p;                   //otherwise,move upward until a node for which the current node is in the left subtree is found
        while (y!=nullptr && x== y->right) {
            x=y;
            y=y->p;
        }
        return y;

    }
    void insert(Node* z) {
        Node*y = nullptr;
        Node*x=root;

        while (x!=nullptr) {        //while we have nodes to go through
            y=x;             //keep track of possible parent node
            if (z->key < x->key)        //find place to insert, compare with other nodes and go either left or right
                x=x->left;
            else
                x = x->right;
        }
        z->p=y;    //assign the parent of the new node
        if (y==nullptr)        //if tree is empty
            root=z;          //the new node becomes the root
        else if (z->key < y->key)     //insert node to the left of parent if it is smaller than the parent
            y->left=z;
        else
            y->right=z;        //insert node to the right of parent if it is larger than the parent
    }

    Node* del(Node* z) {
        Node* y;
        Node* x;
        //find node to remove (y)
        if (z->left == nullptr || z->right == nullptr )         //node has 0 or 1 child
             y=z;
        else                   //if it has 2 children
            y=successor(z);          //successor always has at most 1 child
        //set x to y's child ( or nullptr if y is leaf)
        if (y->left!=nullptr)
          x=y->left;     //y has left child
        else
            x=y->right; //y has right child or is leaf


        if (x!=nullptr)      //update x's parent pointer to y's parent
            x->p=y->p;

        if (y->p == nullptr)       //make x root if y was root
            root=x;
        else if (y==y->p->left)   //if y was lef child
            y->p->left=x;
        else
            y->p->right=x;         //if y was right child

        //if z had 2 children, copy the key of y into z
        if (y!=z) {
            z->key = y->key;
        }


        return y;
    }

    void inorder (Node*x) {        //left child - root - right child
        if (x!=nullptr) {
            inorder(x->left);
            cout<<x->key<<" ";
            inorder(x->right);
        }
    }
};

int main() {
    BSTree T;
    int choice, value;

    do {
        cout << "\n--- BST Menu ---\n";
        cout << "1. Insert a value\n";
        cout << "2. Delete a value\n";
        cout << "3. Search for a value\n";
        cout << "4. Find minimum\n";
        cout << "5. Find maximum\n";
        cout << "6. Inorder traversal\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
            {
                int n;
                cout << "How many values do you want to insert? ";
                cin >> n;

                cout << "Enter " << n << " values:\n";
                for (int i = 0; i < n; i++) {
                    cin >> value;
                    T.insert(new Node(value));
                }

                cout << n << " values inserted.\n";
            }
                break;

            case 2:
                cout << "Enter value to delete: ";
                cin >> value;
                {
                    Node* node = T.search(value);
                    if (node) {
                        Node* removed = T.del(node);
                        delete removed;
                        cout << value << " deleted.\n";
                    } else {
                        cout << "Value not found.\n";
                    }
                }
                break;

            case 3:
                cout << "Enter value to search: ";
                cin >> value;
                if (T.search(value))
                    cout << value << " found in the tree.\n";
                else
                    cout << value << " not found.\n";
                break;

            case 4:
                if (T.root) {
                    cout << "Minimum value: " << T.minimum(T.root)->key << endl;
                } else {
                    cout << "Tree is empty.\n";
                }
                break;

            case 5:
                if (T.root) {
                    cout << "Maximum value: " << T.maximum(T.root)->key << endl;
                } else {
                    cout << "Tree is empty.\n";
                }
                break;

            case 6:
                cout << "Inorder traversal: ";
                T.inorder(T.root);
                cout << endl;
                break;

            case 0:
                cout << "Exiting\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}