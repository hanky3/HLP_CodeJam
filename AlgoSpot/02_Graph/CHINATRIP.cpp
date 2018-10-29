#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <stack>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N, M;
vector<vector<pair<int, int>>> adj;
vector<vector<int>> parentPath;
vector<vector<int>> pathList;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);

	adj = vector<vector<pair<int, int>>>(N, vector<pair<int, int>>());
	parentPath = vector<vector<int>>(N);
	pathList = vector<vector<int>>(N);
	for (int index = 0; index < M; index++) {
		int v1, v2;
		int cost;
		fscanf(fpInput, "%d %d %d", &v1, &v2, &cost);
		v1--;
		v2--;
		adj[v1].push_back(make_pair(v2, cost));
		adj[v2].push_back(make_pair(v1, cost));
	}
}

long long minCityDistance()
{
	vector<long long> dist;
	priority_queue < pair<long long, int>, vector<pair<long long, int>>,
		std::greater<pair<long long, int>>> q;

	dist = vector<long long>(N, numeric_limits<long long>::max());

	dist[0] = 0;
	q.push(make_pair(0, 0));
	while (!q.empty()) {
		int here = q.top().second;
		double cost = q.top().first;
		q.pop();

		if (dist[here] < cost)
			continue;

		for (auto there : adj[here]) {
			long long nextCost = cost + there.second;

			if (nextCost < dist[there.first]) {
				dist[there.first] = nextCost;
				q.push(make_pair(nextCost, there.first));

				parentPath[there.first].clear();
				parentPath[there.first].push_back(here);
			}
			else if (nextCost == dist[there.first]) {
				parentPath[there.first].push_back(here);
			}
		}
	}

	return dist[N - 1];
}


vector<int> generatePath(int from)
{
	if (from == 0)
		return vector<int>(1, from+1);

	vector<int> &ret = pathList[from];
	if (!ret.empty())
		return ret;

	ret.clear();
	for (auto p : parentPath[from]) {
		vector<int> path = generatePath(p);
		path.push_back(from+1);
		
		if (ret.empty() || path < ret)
			ret = path;
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

		long long ret = minCityDistance();
		vector<int> shortestPath = generatePath(N-1);
		if (isFile) {
			printf("%lld\n", ret);
			for (auto city : shortestPath)
				printf("%d ", city);
			printf("\n");
		}
		fprintf(fpOutput, "%lld\n", ret);
		for (auto city : shortestPath)
			fprintf(fpOutput, "%d ", city);
		fprintf(fpOutput, "\n");

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