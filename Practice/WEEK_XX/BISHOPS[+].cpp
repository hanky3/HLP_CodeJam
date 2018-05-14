#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <queue>
#include <assert.h>


using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_W	510
#define MAX_H		510

int N;
unsigned long long initBoard;

const pair<int, int> directions[] = {
	make_pair(-1, -1), make_pair(-1, 1), make_pair(1, -1), make_pair(1, 1)
};

void setBoard(unsigned long long &map, int h, int w)
{
	int index = (h * N) + w;
	map |= (1ull << index);
}

bool isBoardSet(unsigned long long map, int h, int w)
{
	int index = (h * N) + w;
	return (map & (1ull << index));
}

void readInputData()
{
	initBoard = 0;
	fscanf(fpInput, "%d\n", &N);

	for (int h = 0; h < N; h++) {
		for (int w = 0; w < N; w++) {
			char ch;
			fscanf(fpInput, "%c", &ch);
			if (ch == '*') {
				setBoard(initBoard, h, w);
			}
		}
		fscanf(fpInput, "\n");
	}
}

int updateBishop(unsigned long long &myBoard, unsigned long long otherBoard, pair<int, int> pos)
{
	int count = 0;
	for (int i = 0; i < 4; i++) {
		int start = 1;
		while (true) {
			pair<int, int> next = make_pair(pos.first + (start*directions[i].first), pos.second + (start*directions[i].second));
			
			if (next.first < 0 || next.second < 0 || next.first >= N || next.second >= N)
				break;

			if (isBoardSet(otherBoard, next.first, next.second))
				return 0;
			if (isBoardSet(myBoard | initBoard, next.first, next.second))
				break;

			setBoard(myBoard, next.first, next.second);
			count++;
			start++;
		}
	}
	setBoard(myBoard, pos.first, pos.second);
	count++;

	return count;
}

int placeMaxBishop(unsigned long long myBoard, unsigned long long otherBoard)
{
	unsigned long long board = (initBoard | myBoard);
	board |= otherBoard;

	if (board == ((1ull << (N*N)) - 1))
		return 0;

	int ret = 0;

	int updatedCount = numeric_limits<int>::max();
	unsigned long long nextBoard = 0;
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < N; w++) {
			if (isBoardSet(board, h, w))
				continue;

			unsigned long long _my = myBoard;
			int count = updateBishop(_my, otherBoard, make_pair(h, w));
			if (count > 0 && count < updatedCount) {
				updatedCount = count;
				nextBoard = _my;
			}
		}
	}

	if (!nextBoard)
		return 0;

	ret = 1 + placeMaxBishop(otherBoard, nextBoard);
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

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		int ret = placeMaxBishop(0, 0);
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