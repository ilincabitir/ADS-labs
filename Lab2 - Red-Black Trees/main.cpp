#include <iostream>
using namespace std;

enum Color {red, black};

class RBNode {
public:
    int key;
    Color color;
    RBNode *left;
    RBNode *right;
    RBNode *parent;

    RBNode(int k) {
        key = k;
        color = red;
        left=right=parent=nullptr;
    }
    static RBNode* Nil;  //leaf nodes


};

RBNode* RBNode::Nil = []() {
    RBNode* node = new RBNode(0);
    node->color = black;
    node->left = node->right = node->parent = node;
    return node;
}();


class RBTree {
public:
    RBNode* root;

    RBTree() {
        root = RBNode::Nil;
    }

//left rotation

 void LeftRotate(RBNode* x) {
        RBNode* y = x->right;

        x->right = y->left;
        if (y->left != RBNode::Nil)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == RBNode::Nil)
            root=y;
        else if (x==x->parent->left)
                x->parent->left = y;
        else
            x->parent->right=y;

        y->left=x;
        x->parent = y;
    }



    //right rotation
    void RightRotate(RBNode* y) {
        RBNode *x = y->left;

        y->left = x->right;
        if (x->right != RBNode::Nil)
            x->right->parent =y;

        x->parent = y->parent;

        if (y->parent == RBNode::Nil)
            root=x;
        else if (y==y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right=y;
        y->parent=x;
    }

    //insertion:

    void RBInsert(RBNode* z) {
        RBNode* y= RBNode::Nil;
        RBNode* x = root;

        //normal BST insert
        while (x!=RBNode::Nil) {
            y=x;

            if (z->key < x->key)
                x=x->left;
            else
                x=x->right;
        }

        z->parent =y;
        if (y==RBNode::Nil)
            root=z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        //all leaves are black
        z->left = RBNode::Nil;
        z->right = RBNode::Nil;
        z->color=red;  // make color of node red

        RBInsertFixup(z);  //fix possible violations  (red node could have red parent -> we must fix that)
    }

    void RBInsertFixup(RBNode* z) {
        while (z->parent->color == red) {
            if (z->parent == z->parent->parent->left) {
                RBNode* y=z->parent->parent->right;

                if (y->color==red) {          //case 1
                    z->parent->color=black;
                    y->color=black;
                    z->parent->parent->color = red;
                    z=z->parent->parent;
                }
                else {
                    if (z==z->parent->right) {       //case 2
                        z=z->parent;
                        LeftRotate(z);
                    }
                    z->parent->color=black;         //case 3
                    z->parent->parent->color=red;
                    RightRotate(z->parent->parent);
                }
            }
            else{
                //mirror cases
                RBNode* y= z->parent->parent->left;
                if (y->color==red) {
                    z->parent->color=black;
                    y->color=black;
                    z->parent->parent->color=red;
                    z=z->parent->parent;
                }
                else {
                    if (z==z->parent->left) {
                        z=z->parent;
                        RightRotate(z);
                    }

                    z->parent->color=black;
                    z->parent->parent->color=red;
                    LeftRotate(z->parent->parent);
                }
            }
        }
        root->color=black;
    }

    void InorderPrint(RBNode* node) {
        if (node == RBNode::Nil)
            return;

        InorderPrint(node->left);
        cout << node->key << "(" << (node->color == red ? "R" : "B") << ") ";
        InorderPrint(node->right);
    }


    RBNode* Successor(RBNode* z) {
        RBNode* x = z->right;
        while (x != RBNode::Nil) {
            x = x->left;
        }
        return x;
    }

    RBNode* RBDelete(RBNode* z) {
        RBNode* y;       // node to actually remove
        RBNode* x;       // child that will replace y
        Color yOriginalColor;

        // find node to remove (y)
        if (z->left == RBNode::Nil || z->right == RBNode::Nil)
            y = z;                 // z has 0 or 1 child
        else
            y = Successor(z);      // z has 2 children, successor has at most 1 child

        yOriginalColor = y->color;

        // set x to y's child (or Nil if y is leaf)
        if (y->left != RBNode::Nil)
            x = y->left;
        else
            x = y->right;

        // update x's parent pointer
        x->parent = y->parent;

        // splice out y from the tree
        if (y->parent == RBNode::Nil)        // y was root
            root = x;
        else if (y == y->parent->left)       // y was left child
            y->parent->left = x;
        else                                 // y was right child
            y->parent->right = x;

        // if z had 2 children, copy y's key into z
        if (y != z) {
            z->key = y->key;

        }

        // fix R-B properties if a black node was removed
        if (yOriginalColor == black)
            RBDeleteFixup(x);

        return y;
    }


    void RBDeleteFixup(RBNode* x) {
    while (x != root && x->color == black) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;

            // case 1: sibling is red
            if (w->color == red) {
                w->color = black;
                x->parent->color = red;
                LeftRotate(x->parent);
                w = x->parent->right;
            }

            // case 2: sibling black, both children are black
            if (w->left->color == black && w->right->color == black) {
                w->color = red;
                x = x->parent;
            }
            else {
                // case 3: sibling black, right child is black, left child is red
                if (w->right->color == black) {
                    w->left->color = black;
                    w->color = red;
                    RightRotate(w);
                    w = x->parent->right;
                }

                // case 4: sibling is black, right child is red
                w->color = x->parent->color;
                x->parent->color = black;
                w->right->color = black;
                LeftRotate(x->parent);
                x = root;
            }
        }
        else { // mirror case
            RBNode* w = x->parent->left;

            if (w->color == red) {
                w->color = black;
                x->parent->color = red;
                RightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == black && w->left->color == black) {
                w->color = red;
                x = x->parent;
            }
            else {
                if (w->left->color == black) {
                    w->right->color = black;
                    w->color = red;
                    LeftRotate(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = black;
                w->left->color = black;
                RightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = black;
}

    RBNode* Search(int key) {
        RBNode* node = root;
        while (node != RBNode::Nil) {
            if (key == node->key)
                return node;           // found the key
            else if (key < node->key)
                node = node->left;     // go left
            else
                node = node->right;    // go right
        }
        return RBNode::Nil;             // key not found
    }
};


#include <iostream>
using namespace std;

int main() {
    RBTree tree;

    // --- Insertion ---
    int n;
    cout << "How many nodes do you want to insert? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        int key;
        cout << "Enter key to insert: ";
        cin >> key;

        RBNode* node = new RBNode(key);
        tree.RBInsert(node);

        cout << "Tree after inserting " << key << ":\n";
        tree.InorderPrint(tree.root);
        cout << endl;
    }

    // --- Deletion ---
    cout << "\nNow you can delete nodes.\n";
    char choice;
    do {
        int delKey;
        cout << "Enter key to delete: ";
        cin >> delKey;

        // Use search function to find the node
        RBNode* node = tree.Search(delKey);
        if (node != RBNode::Nil) {
            tree.RBDelete(node);
            cout << "Tree after deleting " << delKey << ":\n";
            tree.InorderPrint(tree.root);
            cout << endl;
        } else {
            cout << "Node " << delKey << " not found in the tree.\n";
        }

        cout << "Delete another node? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}

