#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <map>
#include <cmath>

using namespace std;
typedef unsigned long long uint64;

FILE *fpInput;
FILE *fpOutput;

#define MAX_BLOCK_CNT		100010
#define INF						987654321

struct FenwickTree {
	vector<int> trees;

	FenwickTree(int n) : trees(n + 1, 0) {};
	void add(int pos, int val) {
		while (pos < trees.size()) {
			trees[pos] += val;
			pos += (pos & -pos);
		}
	}

	int sum(int pos) {
		int ret = 0;
		while (pos > 0) {
			ret += trees[pos];
			pos &= (pos - 1);
		}
		return ret;
	}
};


int N;
vector<int> blockHeights;
vector<pair<int, int>> blockSorted;
vector<int> blockNum;
FenwickTree erasedQ(0);
pair<int, int> blockHeightCount[MAX_BLOCK_CNT];
long long cache[MAX_BLOCK_CNT];
map<int, int> blockIndex;

void readInputData()
{
	fscanf(fpInput, "%d", &N);

	blockHeights = vector<int>(N, 0);
	blockSorted = vector<pair<int, int>>(N);
	memset(blockHeightCount, 0, sizeof(blockHeightCount));
	memset(cache, -1, sizeof(cache));
	erasedQ = FenwickTree(N);

	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &blockHeights[index]);
		blockSorted[index] = make_pair(index, blockHeights[index]);
		blockHeightCount[blockHeights[index]].second++;
	}

	sort(blockSorted.begin(), blockSorted.end(), [](pair<int, int> &a, pair<int, int> &b) {
		if (a.second == b.second) {
			return (a.first < b.first);
		}
		else
			return (a.second < b.second);
	});

	blockNum.clear();
	int currHeight = 0;
	for (int index = 0; index < blockSorted.size(); index++) {
		auto val = blockSorted[index];
		if (val.second > currHeight) {
			currHeight = val.second;
			blockNum.push_back(index);
			blockHeightCount[val.second].first = index;
		}
	}
}

int erasedBlockNum(int start, int end)
{
	if (start > end)
		swap(start, end);

	return erasedQ.sum(end + 1) - erasedQ.sum(start);
}

long long getCommandCount(int start, int end, int pos, bool goLeft)
{
	long long commandCnt = numeric_limits<long long>::max();
	int here = start;

	if (pos < 0)
		pos++;

	int range = abs(blockSorted[start].first - blockSorted[end].first);
	int erased = erasedBlockNum(blockSorted[start].first, blockSorted[end].first);
	for (int index = start; index <= end; index++) {
		int blockPos = blockSorted[index].first;
		long long currCommands = abs(blockPos - pos);
		currCommands -= erasedBlockNum(pos, blockPos);
		
		if (goLeft) {
			currCommands -= (index - start);
			currCommands += (blockPos - blockSorted[start].first);
			currCommands -= erasedBlockNum(blockSorted[start].first, blockPos);
		}
		else {
			currCommands -= (end - index);
			currCommands += (blockSorted[end].first - blockPos);
			currCommands -= erasedBlockNum(blockPos, blockSorted[end].first);
		}

		currCommands += (range - erased);
		currCommands += 1;
		commandCnt = min(commandCnt, currCommands);
	}

	return commandCnt;
}

void setErasedQ(int start, int end, int val)
{
	for (int index = start; index <= end; index++) {
		int pos = blockSorted[index].first;
		erasedQ.add(pos + 1, val);
	}
}

pair<int, int> findNextHeightRange(int pos)
{
	if (pos < 0)
		return make_pair(0, blockHeightCount[blockSorted[0].second].second - 1);

	int height = blockHeights[pos];
	int prevStart = blockHeightCount[height].first;

	int nextStart = prevStart + blockHeightCount[height].second;
	int nextEnd = nextStart + blockHeightCount[blockSorted[nextStart].second].second - 1;
	return make_pair(nextStart, nextEnd);
}

long long minCommands(int pos)
{
	long long &ret = cache[pos + 1];
	if (ret != -1)
		return ret;

	if (erasedQ.sum(N) == N)
		return ret = 0;

	pair<int, int> heightInfo = findNextHeightRange(pos);

	ret = numeric_limits<long long>::max();

	long long leftCommand = getCommandCount(heightInfo.first, heightInfo.second, pos, true);
	int endPos = blockSorted[heightInfo.second].first;
	setErasedQ(heightInfo.first, heightInfo.second, 1);
	ret = min(ret, leftCommand + minCommands(endPos));
	setErasedQ(heightInfo.first, heightInfo.second, -1);

	endPos = blockSorted[heightInfo.first].first;
	long long rightCommand = getCommandCount(heightInfo.first, heightInfo.second, pos, false);
	setErasedQ(heightInfo.first, heightInfo.second, 1);
	ret = min(ret, rightCommand + minCommands(endPos));
	setErasedQ(heightInfo.first, heightInfo.second, -1);

	return ret;
}


void solveProblem(const char *fileName, bool isFile)
{
	fpInput = stdin;
	fpOutput = stdout;
	if (isFile) {
		fpInput = fopen(fileName, "r");
		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}

	int testCase = 1;
	while (testCase > 0)
	{
		readInputData();
		long long ret = minCommands(-1);
		if (isFile)	
			printf("%lld\n", ret);
		fprintf(fpOutput, "%lld\n", ret);
		testCase--;
	}
	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif
	return 0;
}
