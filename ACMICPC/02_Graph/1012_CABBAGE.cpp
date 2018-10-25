#include <stdio.h>
#include <string.h>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

const pair<int, int> directions[] = { {-1, 0}, {1, 0}, {0, -1}, {0,1} };

int N, M, K;
vector<vector<bool>> cabbageMap;

void readInputData()
{
	fscanf(fpInput, "%d %d %d\n", &M, &N, &K);

	cabbageMap = vector<vector<bool>>(N, vector<bool>(M, 0));

	for (int i = 0; i < K; i++) {
		pair<int,int>pos;
		fscanf(fpInput, "%d %d", &pos.first, &pos.second);
		cabbageMap[pos.second][pos.first] = true;
	}
}

void dfs(pair<int, int> curr, vector<vector<bool>> &maps)
{
	if (!maps[curr.first][curr.second]) return;
	maps[curr.first][curr.second] = false;

	for (auto dir : directions) {
		pair<int, int> next(curr.first + dir.first, curr.second + dir.second);
		if (next.first < 0 || next.first >= N || next.second < 0 || next.second >= M)
			continue;
		if (maps[next.first][next.second])
			dfs(next, maps);
	}
}

int minCabbageWarms()
{
	int ret = 0;
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			if (cabbageMap[h][w]) {
				dfs({ h, w }, cabbageMap);
				ret++;
			}
		}
	}
	return ret;
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
		auto ret = minCabbageWarms();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}

