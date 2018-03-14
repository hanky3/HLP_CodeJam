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

struct BlockPosInfo {
	int x;
	int y;
	bool set;
};

int HEIGHT;
int WIDTH;
int BLOCK_HEIGHT;
int BLOCK_WIDTH;
vector<unsigned char> boardBitInfo;
vector<vector<unsigned char>> allBlockBits;
//unsigned char boards[];
vector<vector<bool>> boards;
vector<vector<bool>> boardVisited;
vector<BlockPosInfo> inputBlock;
vector<vector<pair<int, int>>> blocks;
int cacheCount;
int initUncoverdCnt;


void enableBit(vector<unsigned char> &bitInfo, int index)
{
	bitInfo[index / 8] |= (1 << (index % 8));
}

void disableBit(vector<unsigned char> &bitInfo, int index)
{
	bitInfo[index / 8] &= ~(1 << (index % 8));
}




void readInputData()
{
	fscanf(fpInput, "%d", &HEIGHT);
	fscanf(fpInput, "%d", &WIDTH);
	fscanf(fpInput, "%d", &BLOCK_HEIGHT);
	fscanf(fpInput, "%d", &BLOCK_WIDTH);

	boardBitInfo = vector<unsigned char>(HEIGHT*WIDTH / 8 + 1, 0);

	boards = vector<vector<bool>>(HEIGHT, vector<bool>(WIDTH, true));
	boardVisited = vector<vector<bool>>(HEIGHT, vector<bool>(WIDTH, false));
	for (int hIndex = 0; hIndex < HEIGHT; hIndex++)
	{
		char buf[20 + 1];
		fscanf(fpInput, "%s", buf);

		for (int wIndex = 0; wIndex < WIDTH; wIndex++) {
			int bitIndex = hIndex * WIDTH + wIndex;

			if (buf[wIndex] == '#')
				enableBit(boardBitInfo, bitIndex);

			boards[hIndex][wIndex] = buf[wIndex] == '#' ? true : false;
			initUncoverdCnt += (buf[wIndex] == '#' ? 0 : 1);
		}
	}

	inputBlock.clear();
	blocks.clear();
	for (int hIndex = 0; hIndex < BLOCK_HEIGHT; hIndex++)
	{
		char buf[20 + 1];
		fscanf(fpInput, "%s", buf);

		for (int wIndex = 0; wIndex < BLOCK_WIDTH; wIndex++) {
			BlockPosInfo block;
			block.x = wIndex; block.y = hIndex;
			block.set = (buf[wIndex] == '#') ? true : false;
			inputBlock.push_back(block);
		}
	}

	cacheCount = 0;
}

const int rot[4][3] = {
	{ -1, 1, true }, { -1, -1, false }, { 1, -1, true }, { 1, 1, false }
};


vector<pair<int, int>> adjustBlockInfo(vector<BlockPosInfo> &blockInfos)
{
	vector<pair<int, int>> ret;
	pair<int, int> adjustPos = make_pair(100, 100);

	for (auto val : blockInfos) {
		//if (!val.set)
		//	continue;

		if (val.y < adjustPos.second)
			adjustPos = make_pair(val.x, val.y);
		else if (val.y == adjustPos.second && val.x< adjustPos.first)
			adjustPos = make_pair(val.x, val.y);
	}

	for (auto val : blockInfos) {
		if (!val.set)
			continue;
		
		ret.push_back(make_pair(val.x - adjustPos.first, val.y - adjustPos.second));
	}
	return ret;
}

void makeRotation()
{
	for (int i = 0; i < 4; i++) {
		vector<BlockPosInfo> rotationBlock;
		for (auto val : inputBlock) {
			BlockPosInfo pos = val;
			if (rot[i][2])
				swap(pos.x, pos.y);

			pos.x = rot[i][0] * pos.x;
			pos.y = rot[i][1] * pos.y;
			pos.set = val.set;
			rotationBlock.push_back(pos);
		}

		vector<pair<int, int>> adjustBlock = adjustBlockInfo(rotationBlock);
		sort(adjustBlock.begin(), adjustBlock.end(), [](pair<int, int> &a, pair<int, int> &b) {
			if (a.first < b.first)	return true;
			if (a.first == b.first)
				return (a.second < b.second);

			return false;
		});
		blocks.push_back(adjustBlock);
	}

	sort(blocks.begin(), blocks.end());
	auto itr = unique(blocks.begin(), blocks.end());
	blocks.erase(itr, blocks.end());
}

void makeAllBlocks()
{
	allBlockBits.clear();
	for (auto blockInfo : blocks) {

		for (int h = 0; h < HEIGHT; h++) {
			for (int w = 0; w < WIDTH; w++) {
				vector<unsigned char> blockBits = vector<unsigned char>(HEIGHT*WIDTH / 8 + 1, 0);
				bool available = true;

				for (auto pos : blockInfo) {
					pair<int, int> nextPos;
					nextPos.first = pos.first + w;
					nextPos.second = pos.second + h;

					// range check
					if (nextPos.first < 0 || nextPos.first >= WIDTH ||
						nextPos.second < 0 || nextPos.second >= HEIGHT) {
							available = false;
							break;
					}
					enableBit(blockBits, (nextPos.second * WIDTH + nextPos.first));
				}
				if (available)
					allBlockBits.push_back(blockBits);
			}
		}

	}
}

/*

bool setCover(pair<int, int> currPos, int blockIndex)
{
	bool coverFlag = true;
	for (int i = 0; i < blocks[blockIndex].size(); i++)
	{
		pair<int, int> nextPos = currPos;
		nextPos.first += blocks[blockIndex][i].first;
		nextPos.second += blocks[blockIndex][i].second;

		// range check
		if (nextPos.first < 0 || nextPos.first >= WIDTH ||
			nextPos.second < 0 || nextPos.second >= HEIGHT)
			return false;

		// already set : false
		if (boards[nextPos.second][nextPos.first])
			return false;
	}

	for (int i = 0; i < blocks[blockIndex].size(); i++)
	{
		pair<int, int> nextPos = currPos;
		nextPos.first += blocks[blockIndex][i].first;
		nextPos.second += blocks[blockIndex][i].second;

		boards[nextPos.second][nextPos.first] = true;
	}

	return true;
}

void releaseCover(pair<int, int> currPos, int blockIndex)
{
	for (int i = 0; i < blocks[blockIndex].size(); i++)
	{
		pair<int, int> nextPos = currPos;
		nextPos.first += blocks[blockIndex][i].first;
		nextPos.second += blocks[blockIndex][i].second;

		boards[nextPos.second][nextPos.first] = false;
	}
}

bool boardCovered(pair<int, int> &next)
{
	for (int hIndex = 0; hIndex < HEIGHT; hIndex++)
	{
		for (int wIndex = 0; wIndex < WIDTH; wIndex++)
		{
			if (!boards[hIndex][wIndex] && !boardVisited[hIndex][wIndex]) {
				next = make_pair(wIndex, hIndex);
				return false;
			}
		}
	}
	return true;
}

int boardUncoverdCnt()
{
	int ret = 0;
	for (int hIndex = 0; hIndex < HEIGHT; hIndex++) {
		for (int wIndex = 0; wIndex < WIDTH; wIndex++) {
			if (!boards[hIndex][wIndex])
				ret++;
		}
	}
	return ret;
}

int maxBoardBlocks(int currSum, int uncovered)
{
	int ret = 0;
	pair<int, int> nextPos;

	if (currSum + (uncovered / blocks[0].size()) < cacheCount)
		return currSum;

	if (boardCovered(nextPos))
		return currSum;

	boardVisited[nextPos.second][nextPos.first] = true;
	for (int blockIndex = 0; blockIndex < blocks.size(); blockIndex++)
	{
		if (setCover(nextPos, blockIndex))
		{
			ret = max(ret, maxBoardBlocks(currSum + 1, uncovered+blocks[0].size()));
			releaseCover(nextPos, blockIndex);
		}
	}
	ret = max(ret, maxBoardBlocks(currSum, uncovered));
	cacheCount = max(ret, cacheCount);
	boardVisited[nextPos.second][nextPos.first] = false;

	return ret;
}
*/

bool setCovered(vector<unsigned char> &a, vector<unsigned char> &b)
{
	for (int index = 0; index < a.size(); index++) {
		if (a[index] & b[index])
			return false;
	}

	for (int index = 0; index < a.size(); index++) {
		a[index] |= b[index];
	}

	return true;
}

bool releaseCover(vector<unsigned char> &a, vector<unsigned char> &b)
{
	for (int index = 0; index < a.size(); index++) {
		a[index] &= ~(b[index]);
	}

	return true;
}

int maxBoardBlocks(int start, int sum, int uncovered)
{
	int ret = 0;

	if (sum + (uncovered / blocks[0].size()) < cacheCount)
		return sum;

	if (start >= allBlockBits.size())
		return sum;

	if (setCovered(boardBitInfo, allBlockBits[start])) {
		ret = max(ret, maxBoardBlocks(start+1, sum+1, uncovered+blocks[0].size()));
		releaseCover(boardBitInfo, allBlockBits[start]);
	}

	ret = max(ret, maxBoardBlocks(start + 1, sum, uncovered));
	cacheCount = ret;

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
		readInputData();
		makeRotation();
		makeAllBlocks();

		//int ret = maxBoardBlocks(0, initUncoverdCnt);
		int ret = maxBoardBlocks(0, 0, initUncoverdCnt);
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

