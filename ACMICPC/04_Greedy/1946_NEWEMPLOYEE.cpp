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

int N;
vector<pair<int, int>> scores;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	scores = vector<pair<int, int>>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%d %d", &scores[i].first, &scores[i].second);
	}
	sort(scores.begin(), scores.end(), [&](pair<int, int> a, pair<int, int> b) {
		return (a < b);
	});
}

int maxNewEmployees()
{
	int ret = 0;

	int minRank = N+1;
	for (auto score : scores) {
		int scoreA = score.first;
		int scoreB = score.second;

		if (minRank > scoreB) {
			minRank = scoreB;
			ret++;
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
	while (testCase > 0)	{
		readInputData();
		auto ret = maxNewEmployees();
#if _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);

	return 0;
}
