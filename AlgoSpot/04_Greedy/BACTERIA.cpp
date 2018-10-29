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

int W, H;
int neighborCellIs[MAX_H][MAX_W];
vector<pair<int, int>> liveCells;
queue<pair<pair<int, int>, int>> qLiveCells;


const pair<int, int> directions[] = {
	make_pair(-1, 0), make_pair(1, 0), make_pair(0, -1), make_pair(0, 1)
};


void updateNeighborCells(int(*neighbors)[MAX_W], vector<pair<int, int>> cells, bool killed)
{
	int sgn = 1;
	if (killed)	sgn = -1;
	for (auto pos : cells) {
		int h = pos.first;
		int w = pos.second;

		if (h - 1 >= 0 && neighbors[h - 1][w] >= 0)
			neighbors[h - 1][w] += (sgn);
		if (h + 1 < H && neighbors[h + 1][w] >= 0)
			neighbors[h + 1][w] += (sgn);
		if (w - 1 >= 0 && neighbors[h][w - 1] >= 0)
			neighbors[h][w - 1] += (sgn);
		if (w + 1 < W && neighbors[h][w + 1] >= 0)
			neighbors[h][w + 1] += (sgn);

		if (killed) {
			neighbors[h][w] = -1;
		}
	}
}

void readInputData()
{
	memset(neighborCellIs, -1, sizeof(neighborCellIs));
	fscanf(fpInput, "%d %d\n", &W, &H);
	liveCells.clear();

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			char ch;
			fscanf(fpInput, "%c", &ch);
			if (ch == '*') {
				neighborCellIs[h][w] = 0;
				liveCells.push_back(make_pair(h, w));
				qLiveCells.push(make_pair(make_pair(h, w), 1));
			}
		}
		fscanf(fpInput, "\n");
	}

	updateNeighborCells(neighborCellIs, liveCells, false);
}

int warmExtinctTime()
{
	int ret = 0;
	int liveWarms = liveCells.size();

	while (!qLiveCells.empty()) {
		auto cellInfo = qLiveCells.front().first;
		int step = qLiveCells.front().second;
		qLiveCells.pop();

		if (neighborCellIs[cellInfo.first][cellInfo.second] < 0)
			continue;

		ret = max(ret, step);
		if (neighborCellIs[cellInfo.first][cellInfo.second] == 2 && step == 1) {
			continue;
		}

		neighborCellIs[cellInfo.first][cellInfo.second] = -1;
		liveWarms--;

		for (int dir = 0; dir < 4; dir++) {
			pair<int, int> nextCell = make_pair(cellInfo.first + directions[dir].first, cellInfo.second + directions[dir].second);
			if (nextCell.first < 0 || nextCell.first >= H)
				continue;
			if (nextCell.second < 0 || nextCell.second >= W)
				continue;

			qLiveCells.push(make_pair(nextCell, step+1));
		}
	}

	if (liveWarms > 0)
		ret = -1;

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
		int ret = warmExtinctTime();
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