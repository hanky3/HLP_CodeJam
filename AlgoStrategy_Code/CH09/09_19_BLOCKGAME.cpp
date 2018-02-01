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

#if 0
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
#define L_BLOCK_CNT		6
#define BLOCK_WIDTH	5
#define BLOCK_HEIGHT	5
#define BOARD_CASE_CNT	(1 << 25)

//vector<vector<bool>> gameBoards;
int gameBoards;
vector<vector<pair<int, int>>> BLOCK_INFO;

void initializeBlockInfo()
{
	BLOCK_INFO = vector<vector<pair<int, int>>>(L_BLOCK_CNT);

// L BLOCK
	BLOCK_INFO[0].push_back(make_pair(0, 0));
	BLOCK_INFO[0].push_back(make_pair(1, 0));
	BLOCK_INFO[0].push_back(make_pair(0, 1));

	BLOCK_INFO[1].push_back(make_pair(0, 0));
	BLOCK_INFO[1].push_back(make_pair(1, 0));
	BLOCK_INFO[1].push_back(make_pair(1, 1));

	BLOCK_INFO[2].push_back(make_pair(0, 0));
	BLOCK_INFO[2].push_back(make_pair(0, 1));
	BLOCK_INFO[2].push_back(make_pair(-1, 1));

	BLOCK_INFO[3].push_back(make_pair(0, 0));
	BLOCK_INFO[3].push_back(make_pair(0, 1));
	BLOCK_INFO[3].push_back(make_pair(1, 1));

// === BLOCK
	BLOCK_INFO[4].push_back(make_pair(0, 0));
	BLOCK_INFO[4].push_back(make_pair(1, 0));

	BLOCK_INFO[5].push_back(make_pair(0, 0));
	BLOCK_INFO[5].push_back(make_pair(0, 1));
}

void readGameBoards()
{
	gameBoards = 0;
//	gameBoards = vector<vector<bool>>(BLOCK_HEIGHT, vector<bool>(BLOCK_WIDTH));

	for (int h = 0; h < BLOCK_HEIGHT; h++)
	{
		char boardInfo[BLOCK_WIDTH + 1];
		fscanf(fpInput, "%s", boardInfo);
		for (int w = 0; w < BLOCK_WIDTH; w++)
		{
			if (boardInfo[w] == '#')
			{
				gameBoards |= (1 << (h * 5 + w));
			}
		}
	}
}

bool enableBlockInBoard(int blockIdx, pair<int, int> &pos)
{
	vector<pair<int, int>> &blockInfo = BLOCK_INFO[blockIdx];
	for (int i = 0; i < blockInfo.size(); i++)
	{
		pair<int, int> newBlock;
		newBlock.first = blockInfo[i].first + pos.first;
		newBlock.second = blockInfo[i].second + pos.second;

		// 범위를 벗어난 경우
		if (newBlock.first < 0 || newBlock.first >= BLOCK_WIDTH ||
			newBlock.second < 0 || newBlock.second >= BLOCK_WIDTH)
			return false;

		// 이미 설정된 경우
		int bitIndex = newBlock.second * BLOCK_HEIGHT + newBlock.first;
		if (gameBoards & (1 << bitIndex))
			return false;
	}

	return true;
}

void setBlock(bool flag, int blockIdx, pair<int, int> &pos)
{
	vector<pair<int, int>> &blockInfo = BLOCK_INFO[blockIdx];
	for (int i = 0; i < blockInfo.size(); i++)
	{
		pair<int, int> newBlock;
		newBlock.first = blockInfo[i].first + pos.first;
		newBlock.second = blockInfo[i].second + pos.second;

		int bitIndex = newBlock.second * BLOCK_HEIGHT + newBlock.first;
		if (flag)
			gameBoards |= (1 << bitIndex);
		else
			gameBoards &= ~(1 << bitIndex);

		//gameBoards[newBlock.second][newBlock.first] = flag;
	}
}

unsigned char cache[BOARD_CASE_CNT];
bool canWinGame(int referenceCnt)
{
	unsigned char &ret = cache[gameBoards];
	if (ret != 0xff)
		return  (ret > 0);

	for (int h = 0; h < BLOCK_HEIGHT; h++)
	{
		for (int w = 0; w < BLOCK_WIDTH; w++)
		{
			int bitIndex = h * BLOCK_HEIGHT + w;
			if (gameBoards & (1 << bitIndex))
				continue;

			pair<int, int> nextPos = make_pair(w, h);
	
			for (int blockIdx = 0; blockIdx < L_BLOCK_CNT; blockIdx++)
			{
				if (!enableBlockInBoard(blockIdx, nextPos))
					continue;

				setBlock(true, blockIdx, nextPos);
				bool otherWin = canWinGame(referenceCnt + 1);
				setBlock(false, blockIdx, nextPos);

				if (!otherWin)
				{
					ret = 1;
					return true;
				}
			}
		}
	}
	ret = 0;
	return false;
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
	initializeBlockInfo();
	for (int i = 0; i < testcase; i++)
	{
		// 전역변수 초기화
		memset(cache, 0xff, BOARD_CASE_CNT);
		readGameBoards();


		//pair<int, int> curr = make_pair(0, 0);
		bool ret = canWinGame(1);
		string resultStr = "WINNING";
		if (!ret)
			resultStr = "LOSING";

		fprintf(fpOutput, "%s\n", resultStr.c_str());
		if (isFile)
			printf("%s\n", resultStr.c_str());
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
#endif