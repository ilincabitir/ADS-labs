/** 1.You are given n nodes labeled from 0 to n-1 and a list of undirected edges.
Find how many connected components are in the graph.**/


#include <iostream>
using namespace std;

struct Node {
    int value;
    Node* next;
    Node* rep;
};

Node* nodes[1000];

// MakeSet
Node* MakeSet(int v) {
    Node* node = new Node;
    node->value = v;
    node->next = NULL;
    node->rep = node;
    return node;
}

//Find Set -  faster because we store rep in each node
Node* FindSet(Node* x) {
    return x->rep;
}

// Union
void Union(Node* u, Node* v) {
    Node* ru = FindSet(u);
    Node* rv = FindSet(v);

    if (ru == rv) return;

    // Find end of ru list
    Node* temp = ru;
    while (temp->next != NULL)
        temp = temp->next;

    // Attach rv list to ru
    temp->next = rv;

    // Update representatives of rv list
    Node* curr = rv;
    while (curr != NULL) {
        curr->rep = ru;
        curr = curr->next;
    }
}


// Count connected components
int ConnectedComponents(int n, int edges[][2], int m) {
    // Step 1: MakeSet
    for (int i = 0; i < n; i++) {
        nodes[i] = MakeSet(i);
    }

    // Step 2: Process edges
    for (int i = 0; i < m; i++) {
        int u = edges[i][0];
        int v = edges[i][1];

        if (FindSet(nodes[u]) != FindSet(nodes[v])) {
            Union(nodes[u], nodes[v]);
        }
    }

    // Step 3: Count representatives
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (nodes[i]->rep == nodes[i])
            count++;
    }

    return count;
}


int main() {
    int n = 5;

    int edges[][2] = {
        {0,1}, {1,2}, {3,4}
    };
    int m = 3;

    cout << "Number of connected components: "
         << ConnectedComponents(n, edges, m) << endl;

    return 0;
}