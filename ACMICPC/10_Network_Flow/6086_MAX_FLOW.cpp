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

#define ALPHABET_SIZE		26
#define TOTAL_NODES		(ALPHABET_SIZE*2)
#define INF	987654321

int N;
int src, sink;
int capacity[TOTAL_NODES][TOTAL_NODES];
int flow[TOTAL_NODES][TOTAL_NODES];
vector<vector<int>> adjNodes;

int getNodeId(char node)
{
	if (node >= 'a' && node <= 'z')
		return node - 'a';
	return node - 'A' + ALPHABET_SIZE;
}

void readInputData()
{
	fscanf(fpInput, "%d\n", &N);
	memset(capacity, 0, sizeof(capacity));
	memset(flow, 0, sizeof(flow));
	adjNodes = vector<vector<int>>(TOTAL_NODES);

	src = getNodeId('A');
	sink = getNodeId('Z');

	for (int i = 0; i < N; i++) {
		char start, end;
		int capa;
		fscanf(fpInput, "%c %c %d\n", &start, &end, &capa);
		
		int from = getNodeId(start);
		int to = getNodeId(end);
		capacity[from][to] += capa;
		capacity[to][from] += capa;
		adjNodes[from].push_back(to);
		adjNodes[to].push_back(from);
	}
}

int networkFlow(int source, int sink)
{
	int totalFlow = 0;

	while (true) {
		vector<int> parent(TOTAL_NODES, -1);
		queue<int> q;
		parent[source] = source;

		q.push(source);
		while (!q.empty() && parent[sink] == -1) {
			int here = q.front();
			q.pop();

			for (auto there : adjNodes[here]) {
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
