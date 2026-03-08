#include <iostream>
#include <list>
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

    string toString() {
        return to_string(key) + (color==red ? "(R)" : "(B)");
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

    bool isNil(RBNode* n) {
        return n == RBNode::Nil;
    }

    RBNode* createNode(int key) {
        return new RBNode(key);
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

        RBInsertFixup(z);  //fix possible violations
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

    void inorder() {
        InorderPrint(root);
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

    void indentedDisplay(RBNode* node, int space = 0) {

        if (node == RBNode::Nil)
            return;

        space += 5;

        indentedDisplay(node->right, space);

        cout << endl;

        for (int i = 5; i < space; i++)
            cout << " ";

        cout << node->toString();

        indentedDisplay(node->left, space);
    }

    void indentedDisplay() {
        indentedDisplay(root);
        cout << endl;
    }

    int depth(RBNode* node) {

        if (node == RBNode::Nil)
            return 0;

        int l = depth(node->left);
        int r = depth(node->right);

        return 1 + max(l,r);
    }

    int depth() {
        return depth(root);
    }

    int bh(RBNode* node) {

        if (node == RBNode::Nil)
            return 1;

        int left = bh(node->left);

        if (node->color == black)
            return left + 1;

        return left;
    }

    int bh() {
        return bh(root);
    }

    int maxBlackKey(RBNode* node) {

        if (node == RBNode::Nil)
            return -1;

        int m = max(maxBlackKey(node->left), maxBlackKey(node->right));

        if (node->color == black)
            m = max(m,node->key);

        return m;
    }

    int maxBlackKey() {
        return maxBlackKey(root);
    }

    int maxRedKey(RBNode* node) {

        if (node == RBNode::Nil)
            return -1;

        int m = max(maxRedKey(node->left), maxRedKey(node->right));

        if (node->color == red)
            m = max(m,node->key);

        return m;
    }

    int maxRedKey() {
        return maxRedKey(root);
    }
};

int main() {

    RBTree tree;

    while(true) {

        cout<<"\n------ RB TREE MENU ------\n";
        cout<<"1 Insert node\n";
        cout<<"2 Delete node\n";
        cout<<"3 Search\n";
        cout<<"4 Minimum\n";
        cout<<"5 Maximum\n";
        cout<<"6 Successor\n";
        cout<<"7 Predecessor\n";
        cout<<"8 Display tree\n";
        cout<<"9 Inorder traversal\n";
        cout<<"10 Black height\n";
        cout<<"11 Max black key\n";
        cout<<"12 Max red key\n";
        cout<<"13 Depth\n";
        cout<<"0 Exit\n";

        int op;
        cin>>op;

        if(op==0) break;

        int key;
        RBNode* node;

        switch(op) {

            case 1:
            {
                int n;
                cout << "How many nodes do you want to insert? ";
                cin >> n;

                for (int i = 0; i < n; i++) {
                    cout << "Key: ";
                    cin >> key;
                    tree.RBInsert(tree.createNode(key));
                }
                break;
            }
            case 2:
                cout<<"Key: ";
                cin>>key;
                node=tree.search(tree.root,key);
                if(!tree.isNil(node))
                    tree.del(node);
                else
                    cout<<"Node not found\n";
                break;

            case 3:
                cout<<"Key: ";
                cin>>key;
                node=tree.search(tree.root,key);
                if(!tree.isNil(node))
                    cout<<"Found "<<node->toString()<<endl;
                else
                    cout<<"Not found\n";
                break;

            case 4:
                node=tree.minimum(tree.root);
                if(!tree.isNil(node))
                    cout<<"Minimum "<<node->toString()<<endl;
                break;

            case 5:
                node=tree.maximum(tree.root);
                if(!tree.isNil(node))
                    cout<<"Maximum "<<node->toString()<<endl;
                break;

            case 6:
                cout<<"Key: ";
                cin>>key;
                node=tree.search(tree.root,key);
                node=tree.successor(node);
                if(!tree.isNil(node))
                    cout<<"Successor "<<node->toString()<<endl;
                break;

            case 7:
                cout<<"Key: ";
                cin>>key;
                node=tree.search(tree.root,key);
                node=tree.predecessor(node);
                if(!tree.isNil(node))
                    cout<<"Predecessor "<<node->toString()<<endl;
                break;

            case 8:
                tree.indentedDisplay();
                break;

            case 9:
                tree.inorder();
                cout<<endl;
                break;

            case 10:
                cout<<"Black height "<<tree.bh()<<endl;
                break;

            case 11:
                cout<<"Max black key "<<tree.maxBlackKey()<<endl;
                break;

            case 12:
                cout<<"Max red key "<<tree.maxRedKey()<<endl;
                break;

            case 13:
                cout<<"Depth "<<tree.depth()<<endl;
                break;
        }
    }

    return 0;
}