#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#if 0
using namespace std;

#define MAX_WIDTH 20
#define MAX_HEIGHT 20

int boardInfo[MAX_HEIGHT];
int traverseFlag[MAX_HEIGHT];

int width;
int height;
int blockWidth;
int blockHeight;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

vector<pair<int, int>> blockInfo[4];

void rotateBlock(vector<pair<int, int>> &input, vector<pair<int, int>> &output)
{
	int shift = 0;
	for (auto posInfo : input)
	{
		int newX = posInfo.second * (-1);
		int newY = posInfo.first;

		shift = max(shift, posInfo.second);
		output.push_back(make_pair(newX, newY));
	}

	for (int i = 0; i < output.size(); i++)
		output[i].first += (shift);
}

bool compare(pair<int, int> x, pair<int, int> y)
{
	if (x.second < y.second || (x.second == y.second && x.first < y.first))
		return true;
	else
		return false;
}

void makeBlockInfo(vector<pair<int, int>> input, int &blockCount)
{
	blockInfo[0].clear();
	blockInfo[0] = input;

	for (int index = 1; index <= 3; index++)
	{
		blockInfo[index].clear();
		rotateBlock(blockInfo[index - 1], blockInfo[index]);
	}

	for (int index = 0; index <= 3; index++)
	{
		sort(blockInfo[index].begin(), blockInfo[index].end(), compare);
		pair<int, int> shift = blockInfo[index][0];

		for (int blockIdx = 0; blockIdx < blockInfo[index].size(); blockIdx++)
		{
			blockInfo[index][blockIdx].first -= shift.first;
			blockInfo[index][blockIdx].second -= shift.second;
		}
	}

//	auto last = unique(blockInfo, blockInfo + 4);
}

bool isValidPosition(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return false;

	return true;
}

bool isBlockCoverOK(int x, int y, vector<pair<int, int>> blockPos)
{
	if ((boardInfo[y] >> x) & 0x1)
		return false;

	for (auto pos : blockPos)
	{
		int newX = x + pos.first;
		int newY = y + pos.second;
		if (!isValidPosition(newX, newY))
			return false;

		if ((boardInfo[newY] >> newX) & 0x1)
			return false;
	}
	return true;
}

void setBlockCover(int x, int y, bool flag, vector<pair<int, int>> blockPos)
{
	for (auto pos : blockPos)
	{
		int newX = x + pos.first;
		int newY = y + pos.second;

		if (flag)
			boardInfo[newY] |= (1 << newX);
		else
			boardInfo[newY] &= ~(1 << newX);
	}
}

int getRemainBoardCnt(int &newX, int &newY)
{
	int count = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (!(boardInfo[i] & (1 << j)))
			{
				if (newX < 0)
					newX = j;
				if (newY < 0)
					newY = i;
				count++;
			}
		}
//		count += (width - __popcnt(boardInfo[i]));
	}
	return count;
}

bool findRemainBoardCover(int &newX, int &newY)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if ( !(boardInfo[i] & (1 << j)) )
			{
				newX = j;
				newY = i;
				return true;
			}
		}
	}
	return false;
}

int getMaxBoardCoverCount(int inputCount, int &maxCount)
{
	int ret = inputCount;

	int newX=-1, newY=-1;
	int remainCount = getRemainBoardCnt(newX, newY);
	
	int remainBlockCount = remainCount / blockInfo[0].size();
	if (remainBlockCount + inputCount < maxCount)
		return 1;

	if (remainCount > 0)
	{
		for (int blockIdx = 0; blockIdx < 4; blockIdx++)
		{
			if (isBlockCoverOK(newX, newY, blockInfo[blockIdx]))
			{
				setBlockCover(newX, newY, true, blockInfo[blockIdx]);
				ret = getMaxBoardCoverCount(inputCount + 1, maxCount);
				setBlockCover(newX, newY, false, blockInfo[blockIdx]);
			}
		}
		boardInfo[newY] |= (1 << newX);
		ret = getMaxBoardCoverCount(inputCount, maxCount);
		boardInfo[newY] &= ~(1 << newX);
	}

	maxCount = max(maxCount, inputCount);
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
		fscanf(fpInput, "%d %d %d %d\n", &height, &width, &blockHeight, &blockWidth);
		memset(boardInfo, 0, MAX_HEIGHT*sizeof(int));
		memset(traverseFlag, 0, MAX_HEIGHT*sizeof(int));

		char buf[21];
		for (int y = 0; y < height; y++)
		{
			fscanf(fpInput, "%s", buf);
			for (int x = 0; x < width; x++)
			{
				if (buf[x] == '#')
					boardInfo[y] |= (1 << x);
			}
		}

		vector<pair<int, int>> block;
		for (int y = 0; y < blockHeight; y++)
		{
			fscanf(fpInput, "%s", buf);
			for (int x = 0; x < blockWidth; x++)
			{
				if (buf[x] == '#')
				{
					block.push_back(make_pair(x, y));
				}
			}
		}
		int blockCount = 0;
		makeBlockInfo(block, blockCount);

		int result = 0;
		getMaxBoardCoverCount(0, result);
		fprintf(fpOutput, "%d\n", result);
		if (isFile)
			printf("%d\n", result);
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