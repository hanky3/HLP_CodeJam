#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <limits>
#include <queue>


using namespace std;
FILE *fpInput;
FILE *fpOutput;

#define MAX_V		500

int V;		// node 개수
int capacity[MAX_V][MAX_V];
int flow[MAX_V][MAX_V];
int N, M;
int src, sink;
vector<vector<int>> cowPreference;

void readInputData()
{
	fscanf(fpInput, "%d %d\n", &N, &M);
	memset(capacity, 0, sizeof(capacity));
	memset(flow, 0, sizeof(flow));

	V = N + M + 2;		// 그래프 정점 개수(소 개수 + 축사 개수 + 소스 + 싱크)
	cowPreference = vector<vector<int>>(N);

	for (int i = 0; i < N; i++) {
		int count, barn;
		fscanf(fpInput, "%d", &count);
		for (int j = 0; j < count; j++) {
			fscanf(fpInput, "%d", &barn);
			cowPreference[i].push_back(barn-1);
		}
	}
}

//--------------------------------------------------------------
// 그래프 생성 부분 구현!!
//--------------------------------------------------------------
void makeGraph()
{
	// source
	src = N + M;
	sink = N + M + 1;

	// source - cow
	for (int index = 0; index < N; index++) {
		capacity[src][index] = 1;
	}

	// barn - sink
	for (int index = 0; index < M; index++) {
		capacity[N + index][sink] = 1;
	}

	// cow - barn
	for (int cow = 0; cow < N; cow++) {
		for (auto barn : cowPreference[cow]) {
			capacity[cow][N + barn] = 1;
		}
	}
}

int networkFlow(int source, int sink)
{
	int totalFlow = 0;

	while (true) {
		vector<int> parent(V, -1);
		queue<int> q;
		parent[source] = source;

		q.push(source);
		while (!q.empty() && parent[sink] == -1) {
			int here = q.front();
			q.pop();

			for (int there = 0; there < V; there++) {
				if (capacity[here][there] - flow[here][there] > 0 && parent[there] == -1) {
					parent[there] = here;
					q.push(there);
				}
			}
		}

		if (parent[sink] == -1) break;

		int amount = numeric_limits<int>::max();
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
		makeGraph();
		auto ret = networkFlow(src, sink);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}