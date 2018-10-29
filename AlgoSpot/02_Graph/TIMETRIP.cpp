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

int V, W;
vector<vector<pair<int, int>>> adjHoles;
vector<vector<bool>> reachable;

void readInputData()
{
	fscanf(fpInput, "%d %d", &V, &W);
	adjHoles = vector<vector<pair<int, int>>>(V);
	reachable = vector<vector<bool>>(V, vector<bool>(V, false));

	for (int i = 0; i < W; i++) {
		int src, dest, cost;
		fscanf(fpInput, "%d %d %d", &src, &dest, &cost);
		adjHoles[src].push_back(make_pair(dest, cost));
		reachable[src][dest] = true;
	}
}

void makeReachableDist()
{
	for (int i = 0; i < V; ++i)
		reachable[i][i] = true;

	for (int k = 0; k < V; ++k) {
		for (int i = 0; i < V; ++i) {
			for (int j = 0; j < V; ++j) {
				if (reachable[i][k] && reachable[k][j])
					reachable[i][j] = true;
			}
		}
	}
}

pair<string, string> findHoleDistance(int src, int dest)
{
	vector<long long> upper(V, INF);
	vector<long long> lower(V, -INF);
	upper[src] = 0;
	lower[src] = 0;

	for (int iter = 0; iter < V-1; iter++) {
		for (int here = 0; here < V; here++) {
			for (int i = 0; i < adjHoles[here].size(); i++) {
				int there = adjHoles[here][i].first;
				int cost = adjHoles[here][i].second;

				if (upper[there] > upper[here] + cost) {
					upper[there] = upper[here] + cost;
				}

				if (lower[there] < lower[here] + cost) {
					lower[there] = lower[here] + cost;
				}
			}
		}
	}

	if (lower[dest] < (-INF + 1000000) || upper[dest] > (INF - 1000000))
		return make_pair("UNREACHABLE", "");

	bool upperUpdated = false, lowerUpdated = false;
	for (int here = 0; here < V; here++) {
		for (int i = 0; i < adjHoles[here].size(); i++) {
			int there = adjHoles[here][i].first;
			int cost = adjHoles[here][i].second;

			if (upper[there] > upper[here] + cost) {
				if (reachable[src][here] && reachable[here][dest])
					upperUpdated = true;
			}

			if (lower[there] < lower[here] + cost) {
				if (reachable[src][here] && reachable[here][dest])
					lowerUpdated = true;
			}
		}
	}

	//if (upperUpdated && lowerUpdated)
	//	return make_pair("UNREACHABLE", "");


	string minPath, maxPath;
	char buf[1000];
	sprintf(buf, "%d", lower[dest]);
	maxPath = lowerUpdated ? "INFINITY" : buf;
	sprintf(buf, "%d", upper[dest]);
	minPath = upperUpdated ? "INFINITY" : buf;

	return make_pair(minPath, maxPath);
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

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		makeReachableDist();

		pair<string, string> ret = findHoleDistance(0, 1);
		if (isFile)
			printf("%s %s\n", ret.first.c_str(), ret.second.c_str());
		fprintf(fpOutput, "%s %s\n", ret.first.c_str(), ret.second.c_str());

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