#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
using namespace std;
#if 0
class DisjointSet
{
public :
	DisjointSet(int n) : parent(n), rank(n, 1)
	{
		for (int i = 0; i < n; i++)
			parent[i] = i;
	}

	int find(int u)
	{
		if (u == parent[u])
			return u;

		parent[u] = find(parent[u]);
		return parent[u];
	}

	void merge(int u, int v)
	{
		u = find(u);
		v = find(v);

		if (u == v) return;
		if (rank[u] > rank[v])
			swap(u, v);

		parent[u] = v;
		if (rank[u] == rank[v])
			rank[v]++;
	}
public :
	vector<int> parent, rank;
};
#endif