/**
 4.You are building a system for a social network. Users can become friends over time.
 Friendship is mutual and transitive: If A is friends with B, and B with C → then A, B, C are in the same group

Task: You are given: n users labeled 0 to n-1 and a list of friendship events.
After all friendship events are processed, return:
a. The total number of friend groups
b.The size of each group and their members.

Example:
n = 7 and friendships = [ (0,1), (1,2), (3,4), (5,6)] and expected output:
3 Groups,  Group 1: {0,1,2} size 3 , Group 2: {3,4} size 2, Group 3: {5,6} size 2
*/

#include <iostream>
using namespace std;

const int MAXN = 1000;

int parent[MAXN];
int rankArr[MAXN];

int groupRoot[MAXN];
int groupSize[MAXN];
int groupMembers[MAXN][MAXN];  // group i → members
int groupCount = 0;


// MakeSet
void MakeSet(int x) {
 parent[x] = x;
 rankArr[x] = 0;
}

// FindSet (path compression)
int FindSet(int x) {
 if (x != parent[x])
  parent[x] = FindSet(parent[x]);
 return parent[x];
}

// Link
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

// Union
void Union(int x, int y) {
 Link(FindSet(x), FindSet(y));
}


int main() {
 int n = 7;

 int friendships[][2] = {
  {0,1}, {1,2}, {3,4}, {5,6}
 };
 int m = 4; //nr of frienship pairs

 // Step 1: MakeSet
 for (int i = 0; i < n; i++) {
  MakeSet(i);
 }

 // Step 2: process friendships
 for (int i = 0; i < m; i++) {
  Union(friendships[i][0], friendships[i][1]);
 }

 // Step 3: assign nodes to groups
 for (int i = 0; i < n; i++) {
  int root = FindSet(i);

  // check if root already exists
  int idx = -1;
  for (int j = 0; j < groupCount; j++) {
   if (groupRoot[j] == root) {
    idx = j;
    break;
   }
  }

  // new group
  if (idx == -1) {
   idx = groupCount;
   groupRoot[groupCount] = root;
   groupSize[groupCount] = 0;
   groupCount++;
  }

  // add member
  groupMembers[idx][groupSize[idx]] = i;
  groupSize[idx]++;
 }

 // Step 4: output
 cout << groupCount << " Groups\n\n";

 for (int i = 0; i < groupCount; i++) {
  cout << "Group " << (i + 1) << ": {";

  for (int j = 0; j < groupSize[i]; j++) {
   cout << groupMembers[i][j];
   if (j != groupSize[i] - 1)
    cout << ",";
  }

  cout << "} size " << groupSize[i] << "\n";
 }

 return 0;
}