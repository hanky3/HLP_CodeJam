#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <limits>
#include <map>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define INF		987654321
#define MAX_WIDTH_SIZE		100

int N, M;
int src, sink, totalNodes;

vector<vector<char>> mapInfo;
map<pair<int, int>, int> mapCapacity;
map<pair<int, int>, int> mapFlow;
vector<vector<int>> adjNodes;
pair<int, int> srcPos, sinkPos;

const pair<int, int> directions[] = { {0, 1}, {1, 0} };

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);

	totalNodes = (N *M * 2);
	adjNodes = vector<vector<int>>(totalNodes);

	src = sink = -1;

	mapInfo = vector<vector<char>>(N, vector<char>(M));
	for (int index = 0; index < N; index++) {
		char buf[MAX_WIDTH_SIZE + 1];
		fscanf(fpInput, "%s", buf);
		for (int m = 0; m < M; m++) {
			mapInfo[index][m] = buf[m];
		}
	}
}

int getNodeIndex(int h, int w)
{
	return (h*M + w);
}

void  makeGraph()
{
	int nodeCount = (N*M);
	// in-out node 
	for (int n = 0; n < nodeCount; n++) {
		mapCapacity[{n, nodeCount + n}] = 1;
		mapCapacity[{nodeCount + n, n}] = 0;
		adjNodes[n].push_back(nodeCount + n);
		adjNodes[nodeCount + n].push_back(n);
	}

	// adjacent node
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			if (mapInfo[h][w] == 'K') {
				srcPos = make_pair(h, w);
				src = getNodeIndex(h, w) + nodeCount;
			}
			else if (mapInfo[h][w] == 'H') {
				sinkPos = make_pair(h, w);
				sink = getNodeIndex(h, w);
			}

			if (mapInfo[h][w] == '#') continue;

			int currNode = getNodeIndex(h, w);
			for (auto dir : directions) {
				pair<int, int> nextPos = make_pair(h + dir.first, w + dir.second);
				if (nextPos.first < 0 || nextPos.first >= N || nextPos.second < 0 || nextPos.second >= M)
					continue;
				if (mapInfo[nextPos.first][nextPos.second] == '#')
					continue;

				int nextNode = getNodeIndex(nextPos.first, nextPos.second);
				mapCapacity[{nodeCount + currNode, nextNode}] = INF;
				mapCapacity[{nextNode, nodeCount + currNode}] = 0;
				adjNodes[nodeCount + currNode].push_back(nextNode);
				adjNodes[nextNode].push_back(nodeCount + currNode);

				mapCapacity[{nodeCount + nextNode, currNode}] = INF;
				mapCapacity[{currNode, nodeCount + nextNode}] = 0;
				adjNodes[nodeCount + nextNode].push_back(currNode);
				adjNodes[currNode].push_back(nodeCount + nextNode);
			}
		}
	}

}


int minWallCount(int source, int sink)
{
	/// adjacent check
	if (src == -1 || sink == -1 || (N == 1 && M == 1))
		return -1;
	if (abs(srcPos.first - sinkPos.first) + abs(srcPos.second - sinkPos.second) == 1)
		return -1;

	int totalFlow = 0;
	while (true) {
		vector<int> parent(totalNodes, -1);
		queue<int> q;
		parent[source] = source;

		q.push(source);
		while (!q.empty() && parent[sink] == -1) {
			int here = q.front();
			q.pop();

			for (auto there : adjNodes[here]) {
				if (mapCapacity[{here, there}] - mapFlow[{here, there}] > 0 && parent[there] == -1) {
					parent[there] = here;
					q.push(there);
				}
			}
		}

		if (parent[sink] == -1) break;
		int amount = 1;
		for (int p = sink; p != source; p = parent[p]) {
			mapFlow[{parent[p], p}] += amount;
			mapFlow[{p, parent[p]}] -= amount;
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
	while (testCase > 0)
	{
		readInputData();
		makeGraph();
		auto ret = minWallCount(src, sink);
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