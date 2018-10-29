#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>


using namespace std;

FILE *fpInput;
FILE *fpOutput;
#define MAX_N		10000
#define MAX_V		100
#define INF			987654321

int V, E, N;
vector<vector<int>> adjEdges;
vector<int> destinations;

long long totalPathCount;
vector<vector<int>> parentPath;
long long shownV[MAX_V + 10][MAX_V + 10];

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &V, &E, &N);
	adjEdges = vector<vector<int>>(V, vector<int>(V, INF));
	destinations = vector<int>(N, 0);
	totalPathCount = 0;

	memset(shownV, -1, sizeof(shownV));

	for (int index = 0; index < E; index++) {
		int src, dest, cost;
		fscanf(fpInput, "%d %d %d", &src, &dest, &cost);
		adjEdges[src - 1][dest - 1] = min(adjEdges[src - 1][dest - 1], cost);
		adjEdges[dest - 1][src - 1] = min(adjEdges[dest - 1][src - 1], cost);
	}

	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &destinations[index]);
		destinations[index]--;
	}
}

long long gcd(long long a, long long b)
{
	if (b == 0)	return a;
	return gcd(b, a % b);
}

long long findPathCount(int start, int end)
{
	long long &ret = shownV[start][end];
	if (ret != -1)
		return ret;

	if (start == end) {
		return (ret = 1);
	}

	ret = 0;
	for (auto parent : parentPath[start]) {
		ret += findPathCount(parent, end);
	}

	return ret;
}

vector<long long> findShortestPath(int src)
{
	priority_queue<pair<long long, int>> q;
	vector<long long> dist = vector<long long>(V, numeric_limits<long long>::max());
	parentPath = vector<vector<int>>(V, vector<int>());

	dist[src] = 0ll;
	q.push(make_pair(0ll, src));

	while (!q.empty()) {
		int here = q.top().second;
		double cost = -q.top().first;
		q.pop();

		if (dist[here] < cost)
			continue;

		for (int there = 0; there < V; there++) {
			if (adjEdges[here][there] == INF)
				continue;

			long long thereCost = cost + adjEdges[here][there];

			if (thereCost < dist[there]) {
				dist[there] = thereCost;
				parentPath[there].clear();
				parentPath[there].push_back(here);

				q.push(make_pair(-dist[there], there));
			}
			else if (thereCost == dist[there]) {
				parentPath[there].push_back(here);
			}
		}
	}

	totalPathCount = findPathCount(V - 1, 0);

	return dist;
}

void generateOutput(bool isFile)
{
	for (auto dest : destinations) {
		long long count = shownV[dest][0] * findPathCount(V - 1, dest);
		long long totalCount = totalPathCount;

		if (count <= 0) {
			count = 0;
			totalCount = 1;
		}
		else {
			long long factor = gcd(count, totalCount);
			count /= factor;
			totalCount /= factor;
		}
		fprintf(fpOutput, "%lld/%lld\n", count, totalCount);
		if (isFile)
			printf("%lld/%lld\n", count, totalCount);
	}
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
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		findShortestPath(0);
		generateOutput(isFile);

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}