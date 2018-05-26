#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <cmath>

using namespace std;
typedef unsigned long long uint64;

FILE *fpInput;
FILE *fpOutput;
const int MAX_V = 100;
const int MAX_P = (2 << 20);

int N, M, K;
vector<vector<pair<int, uint64>>> adjNodes;
vector<vector<int>> adjUpperCnt;
vector<vector<int>> adjCost;

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &N, &M, &K);

	adjNodes = vector<vector<pair<int, uint64>>>(N, vector<pair<int, uint64>>());
	adjUpperCnt = vector<vector<int>>(N, vector<int>());
	adjCost = vector<vector<int>>(N, vector<int>());

	for (int index = 0; index < M; index++) {
		int v1, v2;
		int cost, upper;
		fscanf(fpInput, "%d %d %d %d", &v1, &v2, &cost, &upper);
		v1--;
		v2--;

		adjNodes[v1].push_back(make_pair(v2, 0));
		adjNodes[v2].push_back(make_pair(v1, 0));

		adjUpperCnt[v1].push_back(upper);
		adjUpperCnt[v2].push_back(upper);

		adjCost[v1].push_back(cost);
		adjCost[v2].push_back(cost);

	}
}

void updateEdgeCost(int p)
{
	for (int here = 0; here < N; here++) {
		for (int there = 0; there < adjNodes[here].size(); there++) {
			//int cost = adjCost[here][there];
			int upper = adjUpperCnt[here][there];

			uint64 cost = 0;
			if (p > upper) {
				cost = adjCost[here][there] * (uint64)(pow((uint64)(p - upper), 2));
			}
			adjNodes[here][there].second = cost;
		}
	}
}

long long minRoadCost()
{
	vector<uint64> dist;
	priority_queue < pair<uint64, int>, vector<pair<uint64, int>>,
		std::greater<pair<uint64, int>>> q;

	dist = vector<uint64>(N, numeric_limits<uint64>::max());

	dist[0] = 0ll;
	q.push(make_pair(0, 0));
	while (!q.empty()) {
		int here = q.top().second;
		uint64 cost = q.top().first;
		q.pop();

		if (dist[here] < cost)
			continue;

		for (auto there : adjNodes[here]) {
			uint64 nextCost = cost + there.second;

			if (nextCost < dist[there.first]) {
				dist[there.first] = nextCost;
				q.push(make_pair(nextCost, there.first));
			}
		}
	}

	return dist[N - 1];
}

bool canMarathonGo(int p, int maxCost)
{
	updateEdgeCost(p);
	uint64 pathCost = minRoadCost();

	if (pathCost <= maxCost)
		return true;
	return false;
}

int maxParticipants()
{
	int lo = 0;
	int hi = MAX_P;
	int mid = (lo + hi) / 2;
	while (lo + 1 < hi) {
		mid = (lo + hi) / 2;

		if (canMarathonGo(mid, K))
			lo = mid;
		else
			hi = mid;
	}
	return lo;
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
		int ret = maxParticipants();
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);
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
