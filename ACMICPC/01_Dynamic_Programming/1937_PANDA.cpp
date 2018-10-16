#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>


using namespace std;
#define MAX_N			500

FILE *fpInput;
FILE *fpOutput;
int N;
int forestInfo[MAX_N][MAX_N];
int dp[MAX_N+1][MAX_N + 1];
vector < pair<int, pair<int, int>>> sorted;

const pair<int, int> directions[] = { {-1, 0}, {1, 0}, {0,-1}, {0,1} };

void readInputData()
{
	memset(dp, -1, sizeof(dp));
	fscanf(fpInput, "%d", &N);

	sorted.clear();
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < N; w++) {
			fscanf(fpInput, "%d", &forestInfo[h][w]);
			sorted.push_back({ forestInfo[h][w] , {h, w} });
		}
	}
	sort(sorted.begin(), sorted.end());
}

int maxLivingDays(int n, int m)
{
	int &ret = dp[n][m];
	if (ret != -1)	return ret;

	ret = 0;
	int eatValue = forestInfo[n][m];
	for (auto dir : directions) {
		pair<int, int> next(n + dir.first, m + dir.second);
		if (next.first < 0 || next.first >= N || next.second < 0 || next.second >= N)
			continue;

		int nextEatValue = forestInfo[next.first][next.second];
		if (nextEatValue > eatValue)
			ret = max(ret, 1 + (maxLivingDays(next.first, next.second)));
	}
	
	return ret;
}

int maxPandaLivingDay()
{
	int ret = 0;
	int remains = N * N;

	for (auto info : sorted) {
		auto currPos = info.second;
		auto eatVal = info.first;
		if (ret >= remains) break;
		ret = max(ret, 1 + (maxLivingDays(currPos.first, currPos.second)));
		remains--;
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
	//fscanf(fpInput, "%d", &testCase);
	while (testCase > 0) {
		readInputData();
		auto ret = maxPandaLivingDay();
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

