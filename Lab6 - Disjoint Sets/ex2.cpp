/** 2. Detect cycles in an undirected graph. For each edge (u, v):
if find(u) == find(v), a cycle exists
otherwise union(u, v)
*/


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

// FindSet
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

    // Update representative pointers
    Node* curr = rv;
    while (curr != NULL) {
        curr->rep = ru;
        curr = curr->next;
    }
}


// Detect cycle
bool HasCycle(int n, int edges[][2], int m) {
    // Step 1: MakeSet
    for (int i = 0; i < n; i++) {
        nodes[i] = MakeSet(i);
    }

    // Step 2: Process edges
    for (int i = 0; i < m; i++) {
        int u = edges[i][0];
        int v = edges[i][1];

        // Cycle condition
        if (FindSet(nodes[u]) == FindSet(nodes[v])) {
            return true; // cycle found
        }

        // Otherwise merge sets
        Union(nodes[u], nodes[v]);
    }

    return false; // no cycle
}


int main() {
    int n = 5;

    int edges[][2] = {
        {0,1}, {1,2}, {2,0}, {3,4}
    };
    int m = 4;

    if (HasCycle(n, edges, m))
        cout << "Graph contains a cycle\n";
    else
        cout << "Graph does not contain a cycle\n";

    return 0;
}