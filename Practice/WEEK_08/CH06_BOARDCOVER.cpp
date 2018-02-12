#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

int HEIGHT;
int WIDTH;
vector<vector<bool>> boards;

int blocks[4][3][2] = {
	{ { 0, 0 }, { 0, 1 }, { 1, 1 } },
	{ { 0, 0 }, { 0, 1 }, { -1, 1 } },
	{ { 0, 0 }, { 1, 0 }, { 1, 1 } },
	{ { 0, 0 }, { 0, 1 }, { 1, 0 } }
};

bool setCover(pair<int, int> currPos, int blockIndex)
{
	bool coverFlag = true;
	for (int i = 0; i < 3; i++)
	{
		pair<int, int> nextPos = currPos;
		nextPos.first += blocks[blockIndex][i][0];
		nextPos.second += blocks[blockIndex][i][1];

		// range check
		if (nextPos.first < 0 || nextPos.first >= WIDTH ||
			nextPos.second < 0 || nextPos.second >= HEIGHT)
			return false;

		// already set : false
		if (boards[nextPos.second][nextPos.first])
			return false;
	}

	for (int i = 0; i < 3; i++)
	{
		pair<int, int> nextPos = currPos;
		nextPos.first += blocks[blockIndex][i][0];
		nextPos.second += blocks[blockIndex][i][1];

		boards[nextPos.second][nextPos.first] = true;
	}

	return true;
}

void releaseCover(pair<int, int> currPos, int blockIndex)
{
	for (int i = 0; i < 3; i++)
	{
		pair<int, int> nextPos = currPos;
		nextPos.first += blocks[blockIndex][i][0];
		nextPos.second += blocks[blockIndex][i][1];

		boards[nextPos.second][nextPos.first] = false;
	}
}

bool boardCovered(pair<int, int> &next)
{
	for (int hIndex = 0; hIndex < HEIGHT; hIndex++)
	{
		for (int wIndex = 0; wIndex < WIDTH; wIndex++)
		{
			if (!boards[hIndex][wIndex]) {
				next = make_pair(wIndex, hIndex);
				return false;
			}
		}
	}
	return true;
}

bool canCoverBoard(pair<int, int> pos)
{
	return false;
}

int maxBoardCovers()
{
	int ret = 0;
	pair<int, int> nextPos;

	if (boardCovered(nextPos))
		return 1;

	for (int blockIndex = 0; blockIndex < 4; blockIndex++)
	{
		if (setCover(nextPos, blockIndex))
		{
			ret += maxBoardCovers();
			releaseCover(nextPos, blockIndex);
		}
	}

	return ret;
}

void solveProblem(const char *fileName, bool isFile)
{
	if (isFile)
	{
		fpInput = fopen(fileName, "r");

		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}
	else
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	int testCase = 0;

	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		fscanf(fpInput, "%d", &HEIGHT);
		fscanf(fpInput, "%d", &WIDTH);

		boards = vector<vector<bool>>(HEIGHT, vector<bool>(WIDTH, true));

		for (int hIndex = 0; hIndex < HEIGHT; hIndex++)
		{
			char buf[20 + 1];
			fscanf(fpInput, "%s", buf);

			for (int wIndex = 0; wIndex < WIDTH; wIndex++)
				boards[hIndex][wIndex] = buf[wIndex] == '#' ? true : false;
		}

		int ret = maxBoardCovers();
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
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}
