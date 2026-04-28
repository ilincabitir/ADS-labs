/**You are given an n x n matrix isConnected where:
isConnected[i][j] = 1 → city i and city j are directly connected
isConnected[i][j] = 0 → not directly connected

A province is a group of directly or indirectly connected cities.
You are asked to return the total number of provinces.
*/


#include <iostream>
using namespace std;


const int MAXN = 205;

int parent[MAXN];
int rankArr[MAXN];

// MakeSet(x)
// 1. x->parent := x
// 2. x->rank := 0
void MakeSet(int x) {
    parent[x] = x;
    rankArr[x] = 0;
}

// FindSet(x) with path compression
int FindSet(int x) {
    if (x != parent[x]) {
        parent[x] = FindSet(parent[x]);  // path compression
    }
    return parent[x];
}

// Link(x, y)
void Link(int x, int y) {
    if (rankArr[x] > rankArr[y]) {
        parent[y] = x;
    }
    else {
        parent[x] = y;
        if (rankArr[x] == rankArr[y]) {
            rankArr[y]++;
        }
    }
}

// Union(x, y)
void Union(int x, int y) {
    Link(FindSet(x), FindSet(y));
}

// Count provinces (connected components)
int findCircleNum(int n, int isConnected[MAXN][MAXN]) {
    // MakeSet for all nodes
    for (int i = 0; i < n; i++) {
        MakeSet(i);
    }

    // Build unions from adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (isConnected[i][j] == 1) {
                Union(i, j);
            }
        }
    }

    // Count roots
    int provinces = 0;
    for (int i = 0; i < n; i++) {
        if (FindSet(i) == i)
            provinces++;
    }

    return provinces;
}


int main() {
    int n = 3;

    int isConnected[MAXN][MAXN] = {
        {1,1,0},
        {1,1,0},
        {0,0,1}
    };

    cout << "Number of provinces: "
         << findCircleNum(n, isConnected) << endl;

    return 0;
}