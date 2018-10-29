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

int V, E, n, m;
vector<vector<pair<int, int>>> adj;
vector<int> firePlaces;
vector<int> fireStations;

void readInputData()
{
	fscanf(fpInput, "%d %d %d %d", &V, &E, &n, &m);

	adj = vector<vector<pair<int, int>>>(V, vector<pair<int, int>>());
	firePlaces = vector<int>(n);
	fireStations = vector<int>(m);

	for (int i = 0; i < E; i++) {
		int v1, v2, cost;
		fscanf(fpInput, "%d %d %d", &v1, &v2, &cost);
		v1--;
		v2--;

		adj[v1].push_back(make_pair(v2, cost));
		adj[v2].push_back(make_pair(v1, cost));
	}

	for (int i = 0; i < n; i++) {
		fscanf(fpInput, "%d", &firePlaces[i]);
		--firePlaces[i];
	}

	for (int i = 0; i < m; i++) {
		fscanf(fpInput, "%d", &fireStations[i]);
		--fireStations[i];
	}
}

int minFireTruckCost(vector<int> &srcs, vector<int> &dests)
{
	vector<int> dist;
	priority_queue < pair<int, int>, vector<pair<int, int>>,
		std::greater<pair<int, int>>> q;

	dist = vector<int>(V, numeric_limits<int>::max());
	//dist[0] = 1.0f;

	for (auto src : srcs) {
		dist[src] = 0;
		q.push(make_pair(dist[src], src));
	}

	while (!q.empty()) {
		int here = q.top().second;
		double cost = q.top().first;
		q.pop();

		if (dist[here] < cost)
			continue;

		for (auto there : adj[here]) {
			int nextCost = cost + there.second;

			if (nextCost < dist[there.first]) {
				dist[there.first] = nextCost;
				q.push(make_pair(nextCost, there.first));
			}
		}
	}

	int ret = 0;
	for (auto dest : dests) {
		ret += dist[dest];
	}

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

	int testCase = 0;

	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();

		int ret = minFireTruckCost(fireStations, firePlaces);
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