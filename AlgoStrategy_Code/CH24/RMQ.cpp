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

class RMQ
{
public:
	RMQ(const vector<int> &array)
	{
		nodeCount = array.size();
		rangeMin = vector<int>(nodeCount * 4);
	}

	int init(const vector<int> &array, int left, int right, int node)
	{
		if (left == right)
			return rangeMin[node] = array[left];

		int mid = (left + right) / 2;
		int ret = INT_MAX;
		ret = min(ret, init(array, left, mid, node * 2));
		ret = min(ret, init(array, mid+1, right, node * 2+1));
		rangeMin[node] = ret;
		return ret;
	}

	int query(int left, int right, int node, int nodeLeft, int nodeRight)
	{
		if (left < nodeLeft || right > nodeRight) return INT_MAX;

		if (left <= nodeLeft && nodeRight <= right)
			return rangeMin[node];
		
		int mid = (nodeLeft + nodeRight) / 2;
		int ret = INT_MAX;
		ret = min(ret, query(left, right, node * 2, nodeLeft, mid));
		ret = min(ret, query(left, right, node * 2, mid+1, nodeRight));

		return ret;
	}

	int query(int left, int right)
	{
		return query(left, right, 1, 0, nodeCount - 1);
	}

	int update(int index, int newValue, int node, int nodeLeft, int nodeRight)
	{
		if (index < nodeLeft || index > nodeRight)
			return rangeMin[node];

		if (nodeLeft == nodeRight)
			return rangeMin[node] = newValue;

		int mid = (nodeLeft + nodeRight) / 2;
		int ret = INT_MAX;
		ret = min(ret, update(index, newValue, node * 2, nodeLeft, mid));
		ret = min(ret, update(index, newValue, node * 2, mid + 1, nodeRight));
		rangeMin[node] = ret;
		return ret;
	}

	int update(int index, int newValue)
	{
		return update(index, newValue, 1, 0, nodeCount - 1);
	}
public :
	vector<int> rangeMin;
	int nodeCount;
};

struct RangeResult
{
	int size;
	int mostFrequent;
	int leftNumber, leftFreq;
	int rightNumber, rightFreq;
};

RangeResult merge(const RangeResult& a, const RangeResult& b)
{
	RangeResult ret;
	ret.size = a.size + b.size;

	ret.leftNumber = a.leftNumber;
	ret.leftFreq = a.leftFreq;
	if (a.size == a.leftFreq && a.leftNumber == b.leftNumber)
		ret.leftFreq += b.leftFreq;

	ret.rightNumber = b.rightNumber;
	ret.rightFreq = b.rightFreq;
	if (b.size == b.rightFreq && a.rightNumber == b.rightNumber)
		ret.rightFreq += a.rightFreq;

	ret.mostFrequent = max(a.mostFrequent, b.mostFrequent);
	if (a.rightNumber == b.leftNumber)
		ret.mostFrequent = max(ret.mostFrequent, a.rightFreq + b.leftFreq);

	return ret;
}