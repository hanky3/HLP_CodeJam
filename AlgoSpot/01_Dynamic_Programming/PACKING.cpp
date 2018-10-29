#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_STR_SIZE		21
#define MAX_ITEM_COUNT	1001
#define MAX_CAPACITY_SIZE	1001

int N, W;
vector<string> itemNames;
vector<pair<int, int>> itemInfoList;

int cache[MAX_ITEM_COUNT][MAX_CAPACITY_SIZE];
int nextItems[MAX_ITEM_COUNT][MAX_CAPACITY_SIZE];

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &W);
	memset(cache, -1, sizeof(cache));
	memset(nextItems, -1, sizeof(nextItems));

	itemNames = vector<string>(N);
	itemInfoList = vector <pair<int, int>>(N);

	for (int index = 0; index < N; index++) {
		char buf[MAX_STR_SIZE];
		pair<int, int> itemInfo;
		fscanf(fpInput, "%s %d %d", buf, &itemInfo.first, &itemInfo.second);
		itemNames[index] = string(buf);
		itemInfoList[index] = itemInfo;
	}
}

int maxNecessityScore(int start, int capacity)
{
	int &ret = cache[start+1][capacity];
	if (ret != -1)
		return ret;

	ret = 0;
	for (int next = start + 1; next < N; next++) {
		if (itemInfoList[next].first <= capacity) {
			int nextScore = itemInfoList[next].second + maxNecessityScore(next, capacity - itemInfoList[next].first);
			if (nextScore > ret) {
				ret = nextScore;
				nextItems[start+1][capacity] = next;
			}
		}
	}

	return ret;
}

void reconstruct(int start, int capacity, vector<string> &result)
{
	if (nextItems[start+1][capacity] == -1)
		return;
	
	int next = nextItems[start+1][capacity];
	result.push_back(itemNames[next]);
	reconstruct(next, (capacity-itemInfoList[next].first), result);
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
		int ret = maxNecessityScore(-1, W);
		vector<string> items;
		reconstruct(-1, W, items);
		fprintf(fpOutput, "%d %d\n", ret, items.size());
		for (auto item : items)
			fprintf(fpOutput, "%s\n", item.c_str());

		if (isFile) {
			printf("%d %d\n", ret, items.size());
			for (auto item : items)
				printf("%s\n", item.c_str());
		}

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