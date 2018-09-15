#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N, M;
vector<vector<int>> capacity;
vector<vector<int>> flow;
vector<vector<int>> adjDist;
vector<vector<int>> adj;

void addEdgeInfo(int u, int v, int capa, int cost)
{
	adj[u].push_back(v);
	adj[v].push_back(u);
	adjDist[u][v] = cost;
	adjDist[v][u] = -cost;
	capacity[u][v] = capa;
}

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);

	int total = N * 2;
	capacity = vector<vector<int>>(total, vector<int>(total, 0));
	flow = vector<vector<int>>(total, vector<int>(total, 0));
	adjDist = vector<vector<int>>(total, vector<int>(total, 0));
	adj = vector<vector<int>>(total, vector<int>());

	// node(in-out) 연결
	for (int i = 0; i < N; i++) {
		int in = 2 * i;
		int out = 2 * i + 1;
		addEdgeInfo(in, out, 2, 0);
	}

	for (int i = 0; i < M; i++) {
		int u, v, cost;
		fscanf(fpInput, "%d %d %d", &u, &v, &cost);
		u--; v--;
		addEdgeInfo(2*u + 1, 2*v, 1, cost);
		addEdgeInfo(2*v + 1, 2*u, 1, cost);
	}
}

int mcmf()
{
	int ret = 0;
	int total = 2 * N;
	int src = 0;
	int sink = (2 * (N - 1));

	while (1)
	{
		vector<int> prev(total, -1);
		vector<int> dist(total, numeric_limits<int>::max());
		vector<bool> inQ(total, false);

		// SPFA
		queue<int> q;

		dist[src] = 0;
		inQ[src] = true;

		q.push(src);
		while (!q.empty()) {
			int here = q.front();
			q.pop();

			inQ[here] = false;
			for (auto next : adj[here]) {
				if ((capacity[here][next] - flow[here][next]) > 0 && (dist[next] > dist[here] + adjDist[here][next])) {
					dist[next] = dist[here] + adjDist[here][next];
					prev[next] = here;
					if (!inQ[next]) {
						q.push(next);
						inQ[next] = true;
					}
				}
			}
		}

		if (prev[sink] == -1)
			break;

		// 최대 유량
		int flowVal = numeric_limits<int>::max();
		for (int p = sink; p != src; p = prev[p])
			flowVal = min(flowVal, capacity[prev[p]][p] - flow[prev[p]][p]);

		for (int i = sink; i != src; i = prev[i]) {
			ret += flowVal * adjDist[prev[i]][i];
			flow[prev[i]][i] += flowVal;
			flow[i][prev[i]] -= flowVal;
		}
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

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)	{
		readInputData();
		auto ret = mcmf();
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