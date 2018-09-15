#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <limits>
#include <queue>
#include <functional>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

#define ALPHABET_SIZE		26
#define INF	987654321
#define MAX_NODE_SIZE	(25*25)

int N, M, T, D;
vector<vector<int>> mountainHeight;
const pair<int, int> directions[] = { {-1, 0}, {1, 0}, {0, -1},{ 0, 1 } };
int adjCost[MAX_NODE_SIZE][MAX_NODE_SIZE];

int getHeight(char code)
{
	if (code >= 'a' && code <= 'z')
		return code - 'a' + ALPHABET_SIZE;
	return code - 'A';
}

void readInputData()
{
	fscanf(fpInput, "%d %d %d %d\n", &N, &M, &T, &D);
	memset(adjCost, -1, sizeof(adjCost));
	mountainHeight = vector<vector<int>>(N, vector<int>(M));

	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			char code;
			fscanf(fpInput, "%c", &code);
			mountainHeight[h][w] = getHeight(code);
		}
		fscanf(fpInput, "\n");
	}
}

void makeGraph()
{
	int totalNode = (N*M);
	for (int n = 0; n < totalNode; n++)
		for (int m = 0; m < totalNode; m++)
			adjCost[n][m] = INF;

	for (int node = 0; node < totalNode; node++) {
		adjCost[node][node] = 0;
		pair<int, int> pos(node / M, node%M);
		for (int dir = 0; dir < 4; dir++) {
			pair<int, int> there(pos.first + directions[dir].first, pos.second + directions[dir].second);
			if (there.first < 0 || there.first >= N || there.second < 0 || there.second >= M)
				continue;

			int myHeight = mountainHeight[pos.first][pos.second];
			int thereHeight = mountainHeight[there.first][there.second];
			int heightDiff = abs(myHeight - thereHeight);
			if (heightDiff <= T) {
				int cost = 1;
				if (thereHeight > myHeight)
					cost = (int)pow(heightDiff, 2);

				adjCost[node][there.first*M + there.second] = cost;
			}
		}
	}

	for (int k = 0; k < totalNode; k++) {
		for (int u = 0; u < totalNode; u++) {
			for (int v = 0; v < totalNode; v++) {
				adjCost[u][v] = min(adjCost[u][v], adjCost[u][k] + adjCost[k][v]);
			}
		}
	}
}

int findMaxHeight()
{
	int maxHeight = 0;
	maxHeight = mountainHeight[0][0];
	makeGraph();

	int totalNode = (N*M);
	for (int n = 0; n < totalNode; n++) {
		if (adjCost[0][n] + adjCost[n][0] <= D) {
			maxHeight = max(maxHeight, mountainHeight[n / M][n%M]);
		}
	}

	return maxHeight;
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
		auto ret = findMaxHeight();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
