#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

const int HEIGHT = 5;
const int WIDTH = 5;
short cache[(1 << 25) + 1];
//vector<vector<bool>> boards;
vector<int> blockMoves;

int blocks[6][3][2] = {
	// L Block
	{ { 0, 0 }, { 0, 1 }, { 1, 1 } },
	{ { 1, 0 }, { 1, 1 }, { 0, 1 } },
	{ { 0, 0 }, { 1, 0 }, { 1, 1 } },
	{ { 0, 0 }, { 0, 1 }, { 1, 0 } },
	// -- Block
	{ { 0, 0 }, { 0, 1 }, {0xFF, 0xFF} },
	{ { 0, 0 }, { 1, 0 }, { 0xFF, 0xFF } }, 
};

void preCalc()
{
	for (int h = 0; h < HEIGHT; h++)
	{
		for (int w = 0; w < WIDTH; w++)
		{
			for (int blockIdx = 0; blockIdx < 6; blockIdx++)
			{
				int blockMove = 0;
				bool okFlag = true;
				for (int index = 0; index < 3; index++)
				{
					if (blocks[blockIdx][index][0] == 0xFF)
						continue;

					int posX = w + blocks[blockIdx][index][0];
					int posY = h + blocks[blockIdx][index][1];

					if (posX < 0 || posX >= WIDTH ||
						posY < 0 || posY >= HEIGHT)
					{
						okFlag = false;
						break;
					}

					blockMove |= (1 << ((posY * 5) + posX));
				}
				if (okFlag)
					blockMoves.push_back(blockMove);
			}
		}
	}
}

short canWinGame(int board)
{
	short &ret = cache[board];

	if (ret != -1)
		return ret;

	ret = 0;
	for (auto move : blockMoves)
	{
		if ((board & move))
			continue;

		if (!canWinGame(board | move))
		{
			ret = 1;
			break;
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

	preCalc();
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		int board = 0;
		memset(cache, -1, sizeof(cache));

		for (int hIndex = 0; hIndex < HEIGHT; hIndex++)
		{
			char buf[20 + 1];
			fscanf(fpInput, "%s", buf);

			for (int wIndex = 0; wIndex < WIDTH; wIndex++)
			{
				if (buf[wIndex] == '#')
					board |= (1 << ((hIndex * WIDTH) + wIndex));
			}
		}

		short ret = canWinGame(board);
		if (isFile)
			printf("%s\n", ret ? "WINNING" : "LOSING");
		fprintf(fpOutput, "%s\n", ret ? "WINNING" : "LOSING");

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
