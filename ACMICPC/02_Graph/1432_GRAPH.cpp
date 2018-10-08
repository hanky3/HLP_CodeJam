#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <queue>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N;
vector<vector<int>> adj;
vector<int> order;
vector<int> outDegree;

void readInputData()
{
	fscanf(fpInput, "%d\n", &N);
	adj = vector<vector<int>>(N);
	outDegree = vector<int>(N, 0);

	for (int here = 0; here < N; here++) {
		for (int there = 0; there < N; there++) {
			char ch;
			fscanf(fpInput, "%c", &ch);
			if (ch == '1') {
				adj[there].push_back(here);
				outDegree[here]++;
			}
		}
		fscanf(fpInput, "\n");
	}
}

vector<int> topologicalSort()
{
	order = vector<int>(N);
	priority_queue<int> pq;
	for (int n = 0; n < N; n++) {
		if (!outDegree[n])
			pq.push(n);
	}

	int cnt = N;
	while (!pq.empty()) {
		int here = pq.top();
		order[here] = cnt--;
		pq.pop();

		for (auto there : adj[here]) {
			outDegree[there]--;
			if (!outDegree[there])
				pq.push(there);
		}
	}

	for (auto d : outDegree) {
		if (d) return vector<int>(1, -1);
	}

	return order;
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
	while (testCase > 0) {
		readInputData();
		auto ret = topologicalSort();
		for (auto v : ret) {
#ifdef _FILE_
			printf("%d ", v);
#endif
			fprintf(fpOutput, "%d ", v);
		}
		testCase--;
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}

