#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <math.h>

using namespace std;

typedef long long int64;
#define INF		987654321

FILE *fpInput;
FILE *fpOutput;

int N, M;
int srcNode, destNode;
vector<vector<int>> adjEdges;

void readInputData()
{
	fscanf(fpInput, "%d %d" ,&N, &M);
	if (N == 0 && M == 0)
		return;

	fscanf(fpInput, "%d %d", &srcNode, &destNode);
	adjEdges = vector<vector<int>>(N, vector<int>(N, -1));
	//totalPathCount = 0;

	//memset(shownV, -1, sizeof(shownV));
	for (int index = 0; index < M; index++) {
		int src, dest, cost;
		fscanf(fpInput, "%d %d %d", &src, &dest, &cost);
		adjEdges[src][dest] = cost;
	}
}


void removeShortestPath(int start, int end, vector<vector<int>> &parent)
{
	queue<int> q;
	q.push(start);

	vector<bool> visited(N, 0);
	while (!q.empty()) {
		int here = q.front();
		q.pop();

		if (visited[here]) continue;
		visited[here] = true;
		if (here == end)
			continue;

		for (auto p : parent[here]) {
			adjEdges[p][here] = -1;
			q.push(p);
		}
	}

}

int findShortestPath(int src, int dest, bool remove)
{
	priority_queue<pair<int, int>> q;
	vector<int> dist = vector<int>(N, numeric_limits<int>::max());
	vector<vector<int>> parentPath = vector<vector<int>>(N, vector<int>());

	dist[src] = 0;
	q.push(make_pair(0, src));

	while (!q.empty()) {
		int here = q.top().second;
		int cost = -q.top().first;
		q.pop();

		if (dist[here] < cost)
			continue;

		for (int there = 0; there < N; there++) {
			if (adjEdges[here][there] == -1)
				continue;

			int thereCost = cost + adjEdges[here][there];

			if (thereCost < dist[there]) {
				dist[there] = thereCost;
				parentPath[there].clear();
				parentPath[there].push_back(here);

				q.push(make_pair(-dist[there], there));
			}
			else if (thereCost == dist[there]) {
				parentPath[there].push_back(here);
			}
		}
	}
	if (remove)
		removeShortestPath(dest, src, parentPath);
	
	return (dist[dest] == numeric_limits<int>::max()) ? -1 : dist[dest];
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	string outputFileName = string(argv[1]);
	outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
	fpOutput = fopen(outputFileName.c_str(), "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (true) {
		readInputData();
		if (N == 0 && M == 0)
			break;

		findShortestPath(srcNode, destNode, true);
		auto ret = findShortestPath(srcNode, destNode, false);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
