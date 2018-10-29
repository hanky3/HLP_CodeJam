#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <time.h>

using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
#define L_BLOCK_CNT			4
#define MAX_BLOCK_WIDTH	20

int height;
int width;
vector<vector<bool>> gameBoards;

const int blocks[L_BLOCK_CNT][3][2] = {
	{ { 0, 0 }, { 1, 0 }, { 0, 1 } },
	{ { 0, 0 }, { 1, 0 }, { 1, 1 } },
	{ { 0, 0 }, { 0, 1 }, { -1, 1 } },
	{ { 0, 0 }, { 0, 1 }, { 1, 1 } }, 
};

void readGameBoards()
{
	gameBoards.clear();

	fscanf(fpInput, "%d %d", &height, &width);
	gameBoards = vector<vector<bool>>(height, vector<bool>(width));

	for (int h = 0; h < height; h++)
	{
		char boardInfo[MAX_BLOCK_WIDTH + 1];
		fscanf(fpInput, "%s", boardInfo);
		for (int w = 0; w < width; w++)
		{
			if (boardInfo[w] == '#')
				gameBoards[h][w] = true;
			else
				gameBoards[h][w] = false;
		}
	}
}

bool findNextBoardPos(pair<int, int> &curr, pair<int, int> &next)
{
	bool used = false;
	for (int h = curr.second; h < height; h++)
	{
		int w = 0;
		if (!used)
		{
			w = curr.first;
			used = true;
		}
		for (; w < width; w++)
		{
			if (!gameBoards[h][w])
			{
				next = make_pair(w, h);
				return true;
			}
		}
	}
	return false;
}

bool enableBlockInBoard(int blockIdx, pair<int, int> &pos)
{
	for (int i = 0; i < 3; i++)
	{
		pair<int, int> newBlock;
		newBlock.first = blocks[blockIdx][i][0] + pos.first;
		newBlock.second = blocks[blockIdx][i][1] + pos.second;

		// 범위를 벗어난 경우
		if (newBlock.first < 0 || newBlock.first >= width ||
			newBlock.second < 0 || newBlock.second >= height)
			return false;

		// 이미 설정된 경우
		if (gameBoards[newBlock.second][newBlock.first])
			return false;
	}

	return true;
}

void setBlock(bool flag, int blockIdx, pair<int, int> &pos)
{
	for (int i = 0; i < 3; i++)
	{
		pair<int, int> newBlock;
		newBlock.first = blocks[blockIdx][i][0] + pos.first;
		newBlock.second = blocks[blockIdx][i][1] + pos.second;

		gameBoards[newBlock.second][newBlock.first] = flag;
	}
}

int boardCoverCnt(pair<int, int> &currPos)
{
	// 기저사례
	pair<int, int> nextPos;
	if (!findNextBoardPos(currPos, nextPos))
		return 1;

	int ret = 0;
	for (int blockIdx = 0; blockIdx < L_BLOCK_CNT; blockIdx++)
	{
		if (!enableBlockInBoard(blockIdx, nextPos))
			continue;

		setBlock(true, blockIdx, nextPos);
		ret += boardCoverCnt(nextPos);
		setBlock(false, blockIdx, nextPos);
	}

	return ret;
}


void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");

	if (!isFile)
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else
	{
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++)
	{
		// 전역변수 초기화
		readGameBoards();

		pair<int, int> curr = make_pair(0, 0);
		int ret = boardCoverCnt(curr);

		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);

	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}