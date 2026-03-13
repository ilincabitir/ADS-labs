/*
You are receiving a stream of integers and must continuously report the median of the numbers seen so far.
The data structure must support:


ADD x → insert number x

REMOVE x → remove one occurrence of x

MEDIAN → output the current median


If the number of elements is even, return the lower median.

Example
Input
ADD 5
ADD 2
ADD 10
MEDIAN
ADD 7
MEDIAN
REMOVE 5
MEDIAN

Output
5
5
7


 */





#include <iostream>
using namespace std;

enum Color {red, black};

class RBNode {
public:
    int key;
    int size;
    int count;
    Color color;
    RBNode *left;
    RBNode *right;
    RBNode *parent;

    RBNode(int k) {
        key = k;
        count = 1;
        size = 1;
        color = red;
        left=right=parent=nullptr;
    }

    string toString() {
        return to_string(key) + (color==red ? "(R)" : "(B)");
    }

    static RBNode* Nil;  //leaf nodes
};

RBNode* RBNode::Nil = []() {
    RBNode* node = new RBNode(0);
    node->color = black;
    node->count = 0;
    node->size = 0;
    node->left = node->right = node->parent = node;
    return node;
}();

class RBTree {
public:
    RBNode* root;

    RBTree() {
        root = RBNode::Nil;
    }

    bool isNil(RBNode* n) {
        return n == RBNode::Nil;
    }

    RBNode* createNode(int key) {
        return new RBNode(key);
    }
    void updateSize(RBNode* n) {
        if(n==RBNode::Nil) return;
        n->size = n->count + n->left->size + n->right->size;
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
        updateSize(x);
        updateSize(y);
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
        updateSize(y);
        updateSize(x);
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
            else if (z->key > x->key)
                x=x->right;
            else {
                x->count++;
                while(x != RBNode::Nil) {
                    updateSize(x);
                    x = x->parent;
                }
                return;
            }
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
        z->size = z->count;
        RBInsertFixup(z);  //fix possible violations
        while(z != RBNode::Nil){
            updateSize(z);
            z = z->parent;
        }
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



    RBNode* minimum(RBNode* x) {
        while (x->left != RBNode::Nil)
            x = x->left;
        return x;
    }

    RBNode* maximum(RBNode* x) {
        while (x->right != RBNode::Nil)
            x = x->right;
        return x;
    }

    RBNode* successor(RBNode* z) {

        if (z->right != RBNode::Nil)
            return minimum(z->right);

        RBNode* y = z->parent;

        while (y != RBNode::Nil && z == y->right) {
            z = y;
            y = y->parent;
        }

        return y;
    }

    RBNode* predecessor(RBNode* z) {

        if (z->left != RBNode::Nil)
            return maximum(z->left);

        RBNode* y = z->parent;

        while (y != RBNode::Nil && z == y->left) {
            z = y;
            y = y->parent;
        }

        return y;
    }

    RBNode* search(RBNode* node, int key) {

        while (node != RBNode::Nil && key != node->key) {

            if (key < node->key)
                node = node->left;
            else
                node = node->right;
        }

        return node;
    }

    // delete
    RBNode* RBDelete(RBNode* z) {

        RBNode* y;
        RBNode* x;
        Color yOriginalColor;
        if(z->count>1){ // decrement count instead of delete
            z->count--;
            RBNode* temp = z;
            while(temp!=RBNode::Nil){
                updateSize(temp);
                temp=temp->parent;
            }
            return z;
        }


        if (z->left == RBNode::Nil || z->right == RBNode::Nil)
            y = z;
        else
            y = successor(z);

        yOriginalColor = y->color;


        if (y->left != RBNode::Nil)
            x = y->left;
        else
            x = y->right;

        x->parent = y->parent;

        if (y->parent == RBNode::Nil)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        if (y != z)
            z->key = y->key;

        if (yOriginalColor == black)
            RBDeleteFixup(x);



        RBNode* temp = x->parent;
        while(temp!=RBNode::Nil){
            updateSize(temp);
            temp=temp->parent;
        }

        return y;
    }

    void del(RBNode* z) {
        RBDelete(z);
    }

    void RBDeleteFixup(RBNode* x) {

        while (x != root && x->color == black) {

            if (x == x->parent->left) {

                RBNode* w = x->parent->right;

                if (w->color == red) {
                    w->color = black;
                    x->parent->color = red;
                    LeftRotate(x->parent);
                    w = x->parent->right;
                }

                if (w->left->color == black && w->right->color == black) {
                    w->color = red;
                    x = x->parent;
                }

                else {

                    if (w->right->color == black) {
                        w->left->color = black;
                        w->color = red;
                        RightRotate(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = black;
                    w->right->color = black;
                    LeftRotate(x->parent);
                    x = root;
                }
            }

            else {

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

    int kth(RBNode* node,int k){
        if(node==RBNode::Nil) return -1;
        int leftSize = node->left->size;
        if(k <= leftSize) return kth(node->left,k);
        if(k <= leftSize + node->count) return node->key;
        return kth(node->right, k - leftSize - node->count);
    }

};

class MedianStream{
public:
    RBTree tree;

    void ADD(int x){
        tree.RBInsert(new RBNode(x));
    }

    void REMOVE(int x){
        RBNode* node = tree.search(tree.root,x);
        if(node!=RBNode::Nil) tree.del(node);
    }

    void MEDIAN(){
        int n = tree.root->size;
        if(n==0){
            cout << "Empty\n";
            return;
        }
        int k = (n%2==1) ? (n+1)/2 : n/2; // lower median if even
        int median = tree.kth(tree.root,k);
        cout << median << endl;
    }
};

int main(){
    MedianStream ms;
    string cmd;
    cout<<"Enter command (ADD, REMOVE, MEDIAN, anything else to exit):"<<endl;
    while(cin >> cmd){
        cout<<"Enter command (ADD, REMOVE, MEDIAN, anything else to exit):"<<endl;
        if(cmd=="ADD"){
            int x; cin >> x;
            ms.ADD(x);
        }else if(cmd=="REMOVE"){
            int x; cin >> x;
            ms.REMOVE(x);
        }else if(cmd=="MEDIAN"){
            ms.MEDIAN();
        }
        else {
            break;
        }
    }
}