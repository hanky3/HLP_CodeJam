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

#define INF		987654321
#define MAX_V	811

int N, P;
int src, sink;
vector<vector<int>> capacity;
vector<vector<int>> flow;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &P);

	int total = N + N;
	capacity = vector<vector<int>>(total, vector<int>(total, 0));
	flow = vector<vector<int>>(total, vector<int>(total, 0));

	// i번째 정점 in node : i*2, out node : i*2+1
	capacity[0][1] = INF;

	// 도시 in-out capacity 설정
	for (int i = 1; i < N; i++) {
		capacity[2 * i][2 * i + 1] = 1;
	}

	for (int index = 0; index < P; index++) {
		int u, v;
		fscanf(fpInput, "%d %d", &u, &v);
		u--; v--;
		capacity[2 * u + 1][2 * v] = INF;
		capacity[2 * v + 1][2 * u] = INF;
	}
}

int networkFlow(int source, int sink)
{
	int totalFlow = 0;

	while (true) {
		vector<int> parent(N+N, -1);
		queue<int> q;
		parent[source] = source;

		q.push(source);
		while (!q.empty() && parent[sink] == -1) {
			int here = q.front();
			q.pop();

			for (int there = 0; there < capacity.size(); there++) {
				if (capacity[here][there] - flow[here][there] > 0 && parent[there] == -1) {
					parent[there] = here;
					q.push(there);
				}
			}
		}

		if (parent[sink] == -1) break;

		int amount = INF;
		for (int p = sink; p != source; p = parent[p])
			amount = min(capacity[parent[p]][p] - flow[parent[p]][p], amount);

		for (int p = sink; p != source; p = parent[p]) {
			flow[parent[p]][p] += amount;
			flow[p][parent[p]] -= amount;
		}
		totalFlow += amount;
	}

	return totalFlow;
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
		auto ret = networkFlow(0, 2);
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