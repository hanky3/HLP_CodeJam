#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define BASEBALL_PLAYER_CNT	9

FILE *fpInput;
FILE *fpOutput;

int N, K;
vector<pair<int, int>> playerInfo;
vector<int> playerMaxLevel;
vector<int> playerCount;
vector<vector<int>> playerByPosition;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &K);

	playerMaxLevel = vector<int>(BASEBALL_PLAYER_CNT, 0);
	playerCount = vector<int>(BASEBALL_PLAYER_CNT, 0);
	playerByPosition = vector<vector<int>>(BASEBALL_PLAYER_CNT);

	playerInfo = vector<pair<int, int>>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d %d", &playerInfo[index].first, &playerInfo[index].second);
		playerInfo[index].first--;
	}
}

void addPlayer(int position, int level, int &sum, int &selected)
{
	if (level > playerMaxLevel[position]) {
		sum += abs(level - playerMaxLevel[position]);
		playerMaxLevel[position] = level;
	}
	playerByPosition[position].push_back(level);
	playerCount[position]++;
	selected |= (1 << position);
}

void removePlayer(int position, int &sum, int &selected)
{
	auto itr = playerByPosition[position].begin();
	int level = *itr;

	playerByPosition[position].erase(itr);
	playerCount[position]--;

	if (playerCount[position] == 0) {
		playerMaxLevel[position] = 0;
		sum -= level;
		selected &= ~(1 << position);
		return;
	}

	if (level == playerMaxLevel[position]) {
		int nextMaxLevel = *max_element(playerByPosition[position].begin(), playerByPosition[position].end());
		playerMaxLevel[position] = nextMaxLevel;
		sum -= abs(nextMaxLevel - level);
	}
}

int sumPlayerLevel()
{
	const int ALL_SELECTED = ((1 << 9) - 1);
	int maxSum = 0, currSum = 0;
	int selected = 0;

	for (int index = 0; index < playerInfo.size(); index++) {
		if (index >= K) 
			removePlayer(playerInfo[index - K].first, currSum, selected);
		addPlayer(playerInfo[index].first, playerInfo[index].second, currSum, selected);

		if (selected == ALL_SELECTED)
			maxSum = max(maxSum, currSum);
	}

	return maxSum;
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
		int ret = sumPlayerLevel();
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