#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <queue>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MOD	20130203
#define MAX_N	1001 

typedef unsigned long long uint64;

int N, M, A, B;
vector<vector<int>> adjNodes;
vector<int> bossList;
vector<int> bossIndexList;
vector<int> nodeOrder;
vector<int> parentNodes;
vector<int> sumNodes;


vector<bool> visited;
int cache[MAX_N][MAX_N];

void readInputData()
{
	fscanf(fpInput, "%d %d %d %d\n", &N, &M, &A, &B);

	memset(cache, -1, sizeof(cache));
	adjNodes = vector<vector<int>>(N);
	parentNodes = vector<int>(N);
	nodeOrder = vector<int>(N);
	sumNodes = vector<int>(N);

	visited = vector<bool>(N, false);
	for (int index = 0; index < (N - 1); index++) {
		int node1, node2;
		fscanf(fpInput, "%d %d", &node1, &node2);
		node1--;
		node2--;

		adjNodes[node1].push_back(node2);
		adjNodes[node2].push_back(node1);
	}
	int bossNum;

	fscanf(fpInput, "%d", &bossNum);
	bossIndexList = vector<int>(bossNum);
	for (int index = 0; index < bossNum; index++) {
		fscanf(fpInput, "%d", &bossIndexList[index]);
		bossIndexList[index]--;
	}
}

void setParentNodes(int here, int from)
{
	parentNodes[here] = from;
	for (auto child : adjNodes[here]) {
		if (child == from)
			continue;
		setParentNodes(child, here);
	}
}

void setNodeOrder(int here, int from, int &index)
{
	nodeOrder[index++] = here;
	for (auto child : adjNodes[here]) {
		if (child == from)
			continue;
		setNodeOrder(child, here, index);
	}
}

int sumSubNodes(int here, int from)
{
	int ret = 0;
	ret++;

	for (auto there : adjNodes[here]) {
		if (from == there)
			continue;

		ret += sumSubNodes(there, here);
	}
	sumNodes[here] = ret;
	return ret;
}

void generateBossList(int start, int end)
{
	vector<int> parent(N, 0);
	setParentNodes(start, start);
	bossList.clear();

	bossList.push_back(end);
	for (auto p = parentNodes[end]; p != start; p = parentNodes[p]) {
		bossList.push_back(p);
	}
	bossList.push_back(start);
	reverse(bossList.begin(), bossList.end());
}

int findBodyCount(int order, int depth)
{
	int &ret = cache[order][depth];

	if (ret != -1)
		return ret;

	if (depth == 0)
		return (ret = 1);

	if (order >= N)
		return (ret = 0);

	int here = nodeOrder[order];
	ret = 0;

	int nextOrder = order + 1;
	ret += findBodyCount(nextOrder, depth - 1);
	ret %= MOD;
	ret += findBodyCount(order + sumNodes[here], depth);
	ret %= MOD;

	return ret;
}

void generateOutput(bool isFile)
{
	for (auto val : bossIndexList) {
		memset(cache, -1, sizeof(cache));
		sumSubNodes(bossList[val], bossList[val]);
		setParentNodes(bossList[val], bossList[val]);
		int index = 0;
		setNodeOrder(bossList[val], bossList[val], index);

		int ret = findBodyCount(0, M);

		if (isFile)
			printf("%d ", ret);
		fprintf(fpOutput, "%d ", ret);
	}
	if (isFile)
		printf("\n");
	fprintf(fpOutput, "\n");
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
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		generateBossList(A - 1, B - 1);
		generateOutput(isFile);
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