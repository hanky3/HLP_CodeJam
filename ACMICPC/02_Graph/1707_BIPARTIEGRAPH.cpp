#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

int V, E;
vector<vector<int>> adjNodes;
vector<int> nodeGroup;

void readInputData()
{
	fscanf(fpInput, "%d %d", &V, &E);
	adjNodes = vector<vector<int>>(V);
	nodeGroup = vector<int>(V, 0);
	for (int i = 0; i < E; i++) {
		pair<int, int> edges;
		fscanf(fpInput, "%d %d", &edges.first, &edges.second);
		adjNodes[edges.first - 1].push_back(edges.second - 1);
		adjNodes[edges.second - 1].push_back(edges.first - 1);
	}
}

string isBipartiteGraph()
{
	for (int n = 0; n < V; n++) {
		if (nodeGroup[n] != 0) continue;
		
		queue<int> q;
		q.push(n);
		nodeGroup[n] = 1;

		while (!q.empty()) {
			int node = q.front();
			int group = nodeGroup[node];
			q.pop();

			for (auto adj : adjNodes[node]) {
				if (nodeGroup[adj] == 0) {
					nodeGroup[adj] = group ^ 3;
					q.push(adj);
					continue;
				}

				if (nodeGroup[adj] == group)
					return "NO";
			}
		}
	}

	return "YES";
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
	fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = isBipartiteGraph();
#ifdef _FILE_
		printf("%s\n", ret.c_str());
#endif
		fprintf(fpOutput, "%s\n", ret.c_str());
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
