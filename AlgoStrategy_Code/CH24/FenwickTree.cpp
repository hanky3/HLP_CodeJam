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
class FenwickTree
{
public:
	FenwickTree(int n) : tree(n + 1) {}

	int sum(int pos)
	{
		++pos;
		int ret = 0;
		while (pos > 0)
		{
			ret += tree[pos];
			pos &= (pos - 1);
		}
		return ret;
	}

	void add(int pos, int val)
	{
		++pos;
		while (pos < tree.size())
		{
			tree[pos] += val;
			pos += (pos & ~pos);
		}
	}

public:
	vector<int> tree;

};

#endif