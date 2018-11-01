#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <cmath>

using namespace std;
typedef long long int64;
FILE *fpInput;
FILE *fpOutput;
#define INF 987654321

int V, E, K;
vector<vector<pair<int, int>>> adjNodes;

void readInputData()
{
	fscanf(fpInput, "%d %d", &V, &E);
	fscanf(fpInput, "%d", &K);

	adjNodes = vector<vector<pair<int, int>>>(V, vector<pair<int, int>>());
	K--;

	for (int index = 0; index < E; index++) {
		int v1, v2, cost;
		fscanf(fpInput, "%d %d %d", &v1, &v2, &cost);
		v1--;
		v2--;
		adjNodes[v1].push_back(make_pair(v2, cost));
	}
}

vector<int64> calcShortestPath(int start)
{
	vector<int64> dist;
	priority_queue < pair<int64, int>, vector<pair<int64, int>>,
		std::greater<pair<int64, int>>> q;
	dist = vector<int64>(V, INF);
	dist[start] = 0ll;
	q.push(make_pair(0ll, start));

	while (!q.empty()) {
		int here = q.top().second;
		int64 cost = q.top().first;
		q.pop();

		if (dist[here] < cost) continue;

		for (auto there : adjNodes[here]) {
			int64 nextCost = cost + there.second;
			if (nextCost < dist[there.first]) {
				dist[there.first] = nextCost;
				q.push(make_pair(nextCost, there.first));
			}
		}
	}
	return dist;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	fpOutput = fopen("output.txt", "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = calcShortestPath(K);

		for (auto v : ret) {
#ifdef _FILE_
			if (v == INF)
				printf("INF\n");
			else
				printf("%lld\n", v);
#endif
			if (v == INF)
				fprintf(fpOutput, "INF\n");
			else
				fprintf(fpOutput, "%lld\n", v);
		}
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

