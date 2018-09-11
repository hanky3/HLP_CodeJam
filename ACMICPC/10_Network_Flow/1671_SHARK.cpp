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

struct SharkInfo {
	long long size;
	long long speed;
	long long iq;

	bool operator >= (SharkInfo &rhs) {
		return (size >= rhs.size && speed >= rhs.speed && iq >= rhs.iq);
	}
	bool operator == (SharkInfo &rhs) {
		return (size == rhs.size && speed == rhs.speed && iq == rhs.iq);
	}
};

int N, P;
int src, sink;

vector<vector<int>> capacity;
vector<vector<int>> flow;
vector<SharkInfo> sharkInfo;

void readInputData()
{
	fscanf(fpInput, "%d", &N);

	int total = N + N + 2;
	capacity = vector<vector<int>>(total, vector<int>(total, 0));
	flow = vector<vector<int>>(total, vector<int>(total, 0));

	sharkInfo = vector<SharkInfo>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%lld %lld %lld", &sharkInfo[index].size, &sharkInfo[index].speed, &sharkInfo[index].iq);
	}
}

void  makeGraph()
{
	int total = (2 * N) + 2;
	src = total - 2;
	sink = total - 1;

	// src - node
	for (int i = 0; i < N; i++) {
		capacity[src][i] = 2;
	}

	// node - sink
	for (int i = 0; i < N; i++) {
		capacity[N + i][sink] = 1;
	}

	for (int shark1 = 0; shark1 < N; shark1++) {
		for (int shark2 = shark1 + 1; shark2 < N; shark2++) {
			if (sharkInfo[shark1] >= sharkInfo[shark2]) {
				capacity[shark1][N + shark2] = 1;
			}
			else if (sharkInfo[shark2] >= sharkInfo[shark1]) {
				capacity[shark2][N + shark1] = 1;
			}
		}
	}
}


int minSharkSurvive(int source, int sink)
{
	if (N == 1)
		return 1;

	int totalFlow = 0;
	int total = N + N + 2;
	while (true) {
		vector<int> parent(total, -1);
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

	return (N-totalFlow);
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
	while (testCase > 0)
	{
		readInputData();
		makeGraph();
		auto ret = minSharkSurvive(src, sink);
#ifdef _FILE_	
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}