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

int N, M;
int src, sink, total;
vector<vector<int>> adj;
vector<bool> visited;
vector<int> aMatch;
vector<int> bMatch;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);

	adj = vector<vector<int>>(N, vector<int>(M, 0));
	for (int n = 0; n < N; n++) {
		int workCnt;
		fscanf(fpInput, "%d", &workCnt);
		for (int w = 0; w < workCnt; w++) {
			int work;
			fscanf(fpInput, "%d", &work);
			work--;
			adj[n][work] = 1;
		}
	}
}

bool dfs(int a)
{
	if (visited[a])
		return false;

	visited[a] = true;
	for (int b = 0; b < bMatch.size(); b++) {
		if (adj[a][b]) {
			if (bMatch[b] == -1 || dfs(bMatch[b])) {
				aMatch[a] = b;
				bMatch[b] = a;
				return true;
			}
		}
	}
	return false;
}

int bipartiteMatch(int aCount, int bCount)
{
	aMatch = vector<int>(aCount, -1);
	bMatch = vector<int>(bCount, -1);

	int size = 0;
	for (int start = 0; start < aCount; start++) {
		visited = vector<bool>(aCount, false);
		if (dfs(start))
			size++;
	}
	return size;
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
		auto ret = bipartiteMatch(N, M);
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
