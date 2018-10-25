#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N, M, K;
int maxKoreaVote;
vector<pair<int, int>> coderVotes;

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &N, &M, &K);

	maxKoreaVote = 0;
	coderVotes = vector<pair<int, int>>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d %d", &coderVotes[index].first, &coderVotes[index].second);
		maxKoreaVote = max(maxKoreaVote, coderVotes[index].first);
	}
}

bool isKoreaInvited(int factor1)
{
	bool ret = false;

	for (int factor2 = (100 - factor1); factor2 >= 0; factor2--) {
		vector<pair<int, bool>> voteResult(N);

		for (int n = 0; n < N; n++) {
			voteResult[n].first = coderVotes[n].first * factor1 + coderVotes[n].second * factor2;
			voteResult[n].second = false;
			if (coderVotes[n].first == maxKoreaVote)
				voteResult[n].second = true;
		}

		sort(voteResult.begin(), voteResult.end(), [&](pair<int, bool> &a, pair<int, bool> &b) {
			if (a.first == b.first)
				return (a.second > b.second);
			return (a.first > b.first);
		});

		int skip = 0;
		for (int index = 0; index < N; index++) {
			if (voteResult[index].second)	break;
			skip++;
		}

		if (skip >= M)
			return false;

		int factor3 = (100 - factor1 - factor2);
		int remainVotes = K;
		int count = (M - skip);
		if (factor3 == 0)	continue;

		for (int index = skip + 1; index < N; index++) {
			if (voteResult[index].second)
				continue;

			int voteNeeds = (voteResult[skip].first - voteResult[index].first) / factor3 + 1;
			if (voteNeeds > remainVotes)
				break;

			remainVotes -= voteNeeds;
			count--;
			if (count == 0)	
				return false;
		}
	}
	return true;
}

int minKoreaFactor()
{
	int lo = 0, hi = 100, mid;

	if (isKoreaInvited(0))
		return 0;

	while (lo + 1 < hi) {
		mid = (lo + hi) / 2;
		if (isKoreaInvited(mid))
			hi = mid;
		else
			lo = mid;
	}

	return hi;
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

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		int ret = minKoreaFactor();
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