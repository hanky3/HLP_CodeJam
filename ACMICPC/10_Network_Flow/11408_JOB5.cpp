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

#define MAX_TEXT_SIZE		10001
#define ALPHABET_SIZE		26

int N, M;
vector<vector<int>> cost;
vector<vector<int>> flow;
vector<vector<int>> adjDist;
vector<vector<int>> adj;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);

	int total = N + M + 2;
	int S = N + M;
	int T = N + M + 1;
	cost = vector<vector<int>>(total, vector<int>(total, 0));
	flow = vector<vector<int>>(total, vector<int>(total, 0));
	adjDist = vector<vector<int>>(total, vector<int>(total, 0));
	adj = vector<vector<int>>(total, vector<int>());

	// source / node 연결
	for (int i = 0; i < N; i++) {
		cost[S][i] = 1;
		adj[S].push_back(i);
		adj[i].push_back(S);
	}
	
	// work와 T(sink) 연결
	for (int i = 0; i < M; i++) {
		cost[N + i][T] = 1;
		adj[T].push_back(N + i);
		adj[N + i].push_back(T);
	}
	
	for (int i = 0; i < N; i++) {
		int workN;
		fscanf(fpInput, "%d", &workN);
		while (workN-- > 0) {
			int workNo, costNo;
			fscanf(fpInput, "%d %d", &workNo, &costNo);
			workNo--;
			adj[i].push_back(N + workNo);
			adj[N+ workNo].push_back(i);

			adjDist[i][N + workNo] = costNo;
			adjDist[N + workNo][i] = -costNo;

			cost[i][N + workNo] = 1;
		}
	}
}

pair<int, int> mcmf()
{
	pair<int, int> ret(0, 0);
	int total = N + M + 2;
	int S = N + M;
	int T = N + M + 1;
	
	while (1)
	{
		vector<int> prev(total, -1);
		vector<int> dist(total, numeric_limits<int>::max());
		vector<bool> inQ(total, false);

		// SPFA
		queue<int> q;

		dist[S] = 0;
		inQ[S] = true;

		q.push(S);
		while (!q.empty()) {
			int here = q.front();
			q.pop();

			inQ[here] = false;
			for (auto next : adj[here]) {
				if ((cost[here][next] - flow[here][next]) > 0 && (dist[next] > dist[here] + adjDist[here][next])) {
					dist[next] = dist[here] + adjDist[here][next];
					prev[next] = here;
					if (!inQ[next]) {
						q.push(next);
						inQ[next] = true;
					}
				}
			}
		}

		if (prev[T] == -1)
			break;

		// 최대 유량
		int flowVal = numeric_limits<int>::max();
		for (int p = T; p != S; p = prev[p])
			flowVal = min(flowVal, cost[prev[p]][p] - flow[prev[p]][p]);

		for (int i = T; i != S; i = prev[i]) {
			ret.second += flowVal * adjDist[prev[i]][i];
			flow[prev[i]][i] += flowVal;
			flow[i][prev[i]] -= flowVal;
		}

		ret.first++;
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
	while (testCase > 0)
	{
		readInputData();
		auto ret = mcmf();
		if (isFile)
			printf("%d\n%d\n", ret.first, ret.second);
		fprintf(fpOutput, "%d\n%d\n", ret.first, ret.second);
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
