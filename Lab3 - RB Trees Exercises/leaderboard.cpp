/*
You are maintaining a live leaderboard for an online game. Players gain and lose points over time. You must efficiently update scores and answer ranking queries.
Operations
You will receive QQQ operations of the following types:

ADD player score
 Add a new player with the given score.

UPDATE player delta
 Increase or decrease the player's score by delta.

REMOVE player
 Remove the player from the leaderboard.

TOP k
 Return the k players with the highest scores.

Input Example
ADD Alice 120
ADD Bob 90
ADD Carol 150
UPDATE Bob 50
TOP 2
REMOVE Carol
TOP 2


Expected Output
Carol 150
Bob 140
Bob 140
Alice 120



 */


#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

enum Color {red, black};

class RBNode {
public:
    int score;
    string player;

    Color color;
    RBNode *left;
    RBNode *right;
    RBNode *parent;

    RBNode(int s, string p) {
        score = s;
        player = p;
        color = red;
        left = right = parent = nullptr;
    }

    string toString() {
        return player + " " + to_string(score) +
               (color==red ? "(R)" : "(B)");
    }


    static RBNode* Nil;
};

RBNode* RBNode::Nil = []() {
    RBNode* node = new RBNode(0,"");
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

    RBNode* createNode(int score, string player) {
        return new RBNode(score,player);
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
        RBNode* y = RBNode::Nil;
        RBNode* x = root;

        while (x != RBNode::Nil) {
            y = x;
            if (z->score < x->score || (z->score == x->score && z->player < x->player))
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == RBNode::Nil)
            root = z;
        else if (z->score < y->score || (z->score == y->score && z->player < y->player))
            y->left = z;
        else
            y->right = z;

        z->left = RBNode::Nil;
        z->right = RBNode::Nil;
        z->color = red;

        RBInsertFixup(z);
    }

    void RBInsertFixup(RBNode* z){
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

        if (y != z) {
            z->score = y->score;
            z->player = y->player;
        }

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

};

class Leaderboard {
public:
    RBTree tree;
    unordered_map<string, RBNode*> players;


    void ADD (string player,int score) {
        RBNode* node =tree.createNode(score,player);
        tree.RBInsert(node);
        players[player] = node;
    }

    void UPDATE(string player,int delta) {
        RBNode* node = players[player];
        int newScore = node->score + delta;
        tree.del(node);
        RBNode* newNode = tree.createNode(newScore,player);
        tree.RBInsert(newNode);
        players[player] = newNode;
    }
    void REMOVE(string player) {
        RBNode* node = players[player];
        tree.del(node);
        players.erase(player);
    }
    void TOP(int k) {

        RBNode* node = tree.maximum(tree.root);
        while (node != RBNode::Nil && k--) {
            cout << node->player << " " << node->score << endl;
            node = tree.predecessor(node);
        }
        cout << endl;
    }

};


int main() {

    Leaderboard lb;

    string cmd;
    cout<<"Enter command (ADD, UPDATE, REMOVE, TOP, anything else to exit):"<<endl;
    while (cin >> cmd) {
        cout<<"Enter command (ADD, UPDATE, REMOVE, TOP, anything else to exit):"<<endl;
        if (cmd == "ADD") {

            string player;
            int score;
            cin >> player >> score;
            lb.ADD(player,score);
        }

        else if (cmd == "UPDATE") {

            string player;
            int delta;
            cin >> player >> delta;
            lb.UPDATE(player,delta);
        }

        else if (cmd == "REMOVE") {

            string player;
            cin >> player;
            lb.REMOVE(player);
        }

        else if (cmd == "TOP") {

            int k;
            cin >> k;
            lb.TOP(k);
        }
        else {
            break;
        }
    }
}