#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

int N, K, L;
vector<vector<int>> gameMap;
map<pair<int, int>, pair<int, int>> leftRotation, rightRotation;
vector<pair<int, char>> moveCommand;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	fscanf(fpInput, "%d", &K);

	gameMap = vector<vector<int>>(N, vector<int>(N, 0));
	for (int index = 0; index < K; index++) {
		int h, w;
		fscanf(fpInput, "%d %d", &h, &w);
		gameMap[h - 1][w - 1] = 1;
	}
	fscanf(fpInput, "%d", &L);
	moveCommand = vector<pair<int, char>>(L);
	for (int index = 0; index < L; index++) {
		fscanf(fpInput, "%d %c", &moveCommand[index].first, &moveCommand[index].second);
	}

	leftRotation[{-1, 0}] = { 0, -1 };
	leftRotation[{1, 0}] = { 0, 1 };
	leftRotation[{0, -1}] = { 1, 0 };
	leftRotation[{0, 1}] = { -1, 0 };

	rightRotation[{-1, 0}] = { 0, 1 };
	rightRotation[{1, 0}] = { 0, -1 };
	rightRotation[{0, -1}] = { -1, 0 };
	rightRotation[{0, 1}] = { 1, 0 };
}

int findGameTime()
{
	int gameTime = 0;
	pair<int, int> currPos(0, 0);
	pair<int, int> currDir(0, 1);
	queue < pair<int, int>> snakePos;

	auto itr = moveCommand.begin();
	while (true) {
		if (currPos.first < 0 || currPos.first >= N || currPos.second < 0 || currPos.second >= N)
			return gameTime;
		if (gameMap[currPos.first][currPos.second] == 2)
			return gameTime;

		if (gameMap[currPos.first][currPos.second] == 0) {
			if (!snakePos.empty()) {
				gameMap[snakePos.front().first][snakePos.front().second] = 0;
				snakePos.pop();
			}
		}

		gameMap[currPos.first][currPos.second] = 2;
		gameTime++;
		snakePos.push(currPos);


		currPos = { currPos.first + currDir.first, currPos.second + currDir.second };

		if (itr != moveCommand.end()) {
			if (gameTime == itr->first) {
				if (itr->second == 'L')
					currDir = leftRotation[currDir];
				else
					currDir = rightRotation[currDir];
				itr++;
			}
		}
	}
	return -1;
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
		auto ret = findGameTime();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
