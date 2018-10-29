#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define INF 987654321
#define MAX_V		500

int V, E;
vector<int> drunkenTimes;
vector<vector<int>> minFloydPath;
vector<vector<int>> minFinalDistance;
vector<pair<int, int>> startEndPos;

void readInputData()
{
	fscanf(fpInput, "%d %d", &V, &E);
	minFloydPath = vector<vector<int>>(V, vector<int>(V, INF));
	minFinalDistance = vector<vector<int>>(V, vector<int>(V, INF));
	drunkenTimes = vector<int>(V);

	for (int i = 0; i < V; i++) {
		fscanf(fpInput, "%d", &drunkenTimes[i]);
	}

	for (int i = 0; i < E; i++) {
		int src, dest, cost;
		fscanf(fpInput, "%d %d %d", &src, &dest, &cost);
		minFloydPath[src-1][dest-1] = cost;
		minFloydPath[dest - 1][src - 1] = cost;
	}

	int queryCnt;
	fscanf(fpInput, "%d", &queryCnt);
	startEndPos = vector<pair<int, int>>(queryCnt);
	for (int i = 0; i < queryCnt; i++) {
		fscanf(fpInput, "%d %d", &startEndPos[i].first, &startEndPos[i].second);
		startEndPos[i].first--;
		startEndPos[i].second--;

		if (startEndPos[i].first > startEndPos[i].second)
			swap(startEndPos[i].first, startEndPos[i].second);
	}
}

void makeFloydDistance()
{
	vector<pair<int, int>> drunkenTList;

	for (int i = 0; i < V; ++i) {
		minFloydPath[i][i] = 0;
		drunkenTList.push_back(make_pair(drunkenTimes[i], i));
	}

	for (int i = 0; i < V; ++i) {
		for (int j = 0; j < V; ++j) {
			if (i == j)
				minFinalDistance[i][j] = 0;
			else
				minFinalDistance[i][j] = minFloydPath[i][j];
		}
	}

	sort(drunkenTList.begin(), drunkenTList.end());

	for (int k = 0; k < V; ++k) {
		int nextK = drunkenTList[k].second;
		for (int i = 0; i < V; ++i) {
			for (int j = 0; j < V; ++j) {
				minFloydPath[i][j] = min(minFloydPath[i][j], minFloydPath[i][nextK] + minFloydPath[nextK][j]);
				minFinalDistance[i][j] = min(minFinalDistance[i][j], minFloydPath[i][nextK] + drunkenTimes[nextK] + minFloydPath[nextK][j]);
			}
		}
	}
}

void printMinimumTimes(bool isFile)
{
	for (auto val : startEndPos) {
		int solveTime = minFinalDistance[val.first][val.second];
		if (isFile)
			printf("%d\n", solveTime);
		fprintf(fpOutput, "%d\n", solveTime);
	}
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
	//fscanf(fpInput, "%d", &testCase);

	readInputData();
	makeFloydDistance();
	printMinimumTimes(isFile);

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