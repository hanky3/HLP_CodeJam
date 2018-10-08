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
#define INF 987654321

int n, m;
vector<vector<int>> adjDist;

void readInputData()
{
	fscanf(fpInput, "%d\n", &n);
	fscanf(fpInput, "%d\n", &m);
	adjDist = vector<vector<int>>(n, vector<int>(n, INF));


	for (int index = 0; index < m; index++) {
		int a, b, c;
		fscanf(fpInput, "%d %d %d", &a, &b, &c);
		adjDist[a - 1][b - 1] = min(adjDist[a-1][b-1], c);
	}
}


void calcFloydDistance()
{
	for (int i = 0; i < n; i++)
		adjDist[i][i] = 0;

	for (int k = 0; k < n; ++k) {
		for (int i = 0; i < n; ++i) {
			if (adjDist[i][k] == INF) continue;
			for (int j = 0; j < n; ++j) {
				adjDist[i][j] = min(adjDist[i][j], adjDist[i][k] + adjDist[k][j]);
			}
		}
	}
}

void printMinDistCost()
{
	for (int here = 0; here < n; ++here) {
		for (int there = 0; there < n; ++there) {
			if (adjDist[here][there] == INF)
				adjDist[here][there] = 0;
#ifdef _FILE_
			printf("%d ", adjDist[here][there]);
#endif
			fprintf(fpOutput, "%d ", adjDist[here][there]);
		}
#ifdef _FILE_
		printf("\n");
#endif
		fprintf(fpOutput, "\n");
	}
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
		calcFloydDistance();
		printMinDistCost();
		testCase--;
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}

