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
        else
            if (x==x->parent->left)
                x->parent->left = y;
        else
            x->parent->right=y;

        x->left=x;
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
        else if (y=y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right=y;
        y->left=x;
    }
};