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
const int INF = std::numeric_limits<int>::max();
FILE *fpInput = NULL;
FILE *fpOutput = NULL;

class RMQ
{
public:
	RMQ(const vector<int> &array)
	{
		nodeCount = array.size();
		rangeValues = vector<pair<int, int>>(nodeCount * 4);
		init(array, 0, nodeCount - 1, 1);

	}

	pair<int, int> init(const vector<int> &array, int left, int right, int node)
	{
		if (left == right)
			return rangeValues[node] = make_pair(array[left], array[left]);

		int mid = (left + right) / 2;

		pair<int, int> leftValue = init(array, left, mid, node * 2);
		pair<int, int> rightValue = init(array, mid + 1, right, node * 2 + 1);
		rangeValues[node] = make_pair(min(leftValue.first, rightValue.first), max(leftValue.second, rightValue.second));

		return rangeValues[node];
	}

	pair<int, int> query(int left, int right, int node, int nodeLeft, int nodeRight)
	{
		if (nodeRight < left || right < nodeLeft) return pair<int, int>(INF, -1);

		if (left <= nodeLeft && nodeRight <= right)
			return rangeValues[node];

		int mid = (nodeLeft + nodeRight) / 2;
		pair<int, int> leftValue = query(left, right, node * 2, nodeLeft, mid);
		pair<int, int> rightValue = query(left, right, node * 2 + 1, mid + 1, nodeRight);
		pair<int, int> ret = make_pair(min(leftValue.first, rightValue.first), max(leftValue.second, rightValue.second));

		return ret;
	}

	pair<int, int> query(int left, int right)
	{
		return query(left, right, 1, 0, nodeCount - 1);
	}

public:
	vector<pair<int, int>> rangeValues;
	int nodeCount;
};

int N, Q;

void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");

	if (!isFile)
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else
	{
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++)
	{

		// 전역변수 초기화

		fscanf(fpInput, "%d %d\n", &N, &Q);

		vector<int> arrayValue = vector<int>(N);
		for (int index = 0; index < N; index++)
			fscanf(fpInput, "%d", &arrayValue[index]);

		RMQ rmq(arrayValue);
		for (int index = 0; index < Q; index++)
		{
			int left, right;
			fscanf(fpInput, "%d %d", &left, &right);

			//pair<int, int> ret = rmq.query(left, right);
			right++;
			pair<int, int> ret;
			ret.first = *std::min_element(arrayValue.begin() + left, arrayValue.begin() + right);
			ret.second = *std::max_element(arrayValue.begin() + left, arrayValue.begin() + right);
			fprintf(fpOutput, "%d\n", ret.second - ret.first);
			if (isFile) printf("%d\n", ret.second - ret.first);
		}

	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif