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

#define PLAYER_X	0
#define PLAYER_Y	1
#define DRAW		2

const int BOARD_BITS = (1 << 9);
int cacheWin[BOARD_BITS][BOARD_BITS][2];
int xBoard, oBoard;
int firstPlayer;

int winBoard[8] = {
	( (1 << 0) | (1 << 1) | (1 << 2) ),
	((1 << 3) | (1 << 4) | (1 << 5)),
	((1 << 6) | (1 << 7) | (1 << 8)),
	((1 << 0) | (1 << 3) | (1 << 6)),
	((1 << 1) | (1 << 4) | (1 << 7)),
	((1 << 2) | (1 << 5) | (1 << 8)),
	((1 << 0) | (1 << 4) | (1 << 8)),
	((1 << 2) | (1 << 4) | (1 << 6)),
};

const char* playerStr[3] = {
	"x", "o", "TIE"
};

void readInputData()
{
	char buf[4];
	memset(cacheWin, -1, sizeof(cacheWin));

	xBoard = 0;
	oBoard = 0;
	firstPlayer = PLAYER_X;

	int xBoardCnt = 0, oBoardCnt = 0;
	for (int h = 0; h < 3; h++) {
		fscanf(fpInput, "%s", buf);

		for (int w = 0; w < 3; w++) {
			if (buf[w] == 'x') {
				xBoard |= (1 << ((h * 3) + w));
				xBoardCnt++;
			}
			else if (buf[w] == 'o') {
				oBoard |= (1 << ((h * 3) + w));
				oBoardCnt++;
			}
		}
	}

	if (xBoardCnt > oBoardCnt)
		firstPlayer = PLAYER_Y;
}

bool checkWinGame(int board)
{
	int count = sizeof(winBoard) / sizeof(int);
	for (int index = 0; index < count; index++) {
		if ((board & winBoard[index]) == winBoard[index])
			return true;
	}

	return false;
}

int tictactoe(int board1, int board2, int player)
{
	// 기저사례
	if (checkWinGame(board1))
		return PLAYER_X;

	if (checkWinGame(board2))
		return PLAYER_Y;

	int boardTotal = (board1 | board2);
	if ((boardTotal & (BOARD_BITS - 1)) == (BOARD_BITS - 1))
		return DRAW;

	int &ret = cacheWin[board1][board2][player];

	if (ret > -1)
		return ret;

	ret = (player ^ 1);
	bool drawGame = false;
	for (int pos = 0; pos < 9; pos++) {
		if (boardTotal & (1 << pos))
			continue;

		int nextWins = player;
		if (player == PLAYER_X) {
			nextWins = tictactoe(board1 | (1 << pos), board2, (player ^ 1));
		}
		else {
			nextWins = tictactoe(board1, board2 | (1 << pos), (player ^ 1));
		}

		if (nextWins == player) {
			ret = player;
			return ret;
		}
		else if (nextWins == DRAW) {
			drawGame = true;
		}
	}

	if (drawGame)
		ret = DRAW;

	return ret;
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

		int ret = tictactoe(xBoard, oBoard, firstPlayer);
		if (isFile)
			printf("%s\n", playerStr[ret]);
		fprintf(fpOutput, "%s\n", playerStr[ret]);

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