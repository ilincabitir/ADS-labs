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
};


int main() {
    RBTree tree;

    int keys[] = {10, 20, 30, 15, 25, 5};
    for (int k : keys) {
        RBNode* node = new RBNode(k);
        tree.RBInsert(node);
    }

    cout << "Inorder traversal:\n";
    tree.InorderPrint(tree.root);
    cout << endl;

    return 0;
}

