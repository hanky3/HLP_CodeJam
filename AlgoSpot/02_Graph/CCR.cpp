#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int H, W;
vector<vector<bool>> map;
const pair<int, int> directions[4] = {
	{ 1, 0 }, { 0, 1 }, { -1, 0 }, {0, -1}
};
string numberPaths[11];


void generateNumPaths()
{
	numberPaths[0] = "0123";
	numberPaths[1] = "0";
	numberPaths[2] = "010301";
	numberPaths[3] = "01033";
	numberPaths[4] = "0102";
	numberPaths[5] = "01031";
	numberPaths[6] = "01231";
	numberPaths[7] = "010";
	numberPaths[8] = "01233";
	numberPaths[9] = "010323";
	numberPaths[10] = "032";
}


void readInputData()
{
	fscanf(fpInput, "%d %d\n", &H, &W);
	map = vector<vector<bool>>(H, vector<bool>(W, false));

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			char ch;
			fscanf(fpInput, "%c", &ch);
			map[h][w] = (ch == '*') ? true : false;
		}
		fscanf(fpInput, "\n");
	}
}

int calcNumber(string path)
{
	for (int num = 0; num <= 10; num++) {
		if (path == numberPaths[num])
			return (num < 10) ? num : 4;
	}
	return -1;
}

void findNumber(int prevDir, int currDir, pair<int, int> here, string &path)
{
	if (!map[here.first][here.second])
		return;

	map[here.first][here.second] = false;
	if (prevDir != currDir)
		path.push_back(currDir + '0');

	for (int dir = 0; dir < 4; dir++) {
		pair<int, int> next(here.first + directions[dir].first, here.second + directions[dir].second);
		if (next.first < 0 || next.first >= H || next.second < 0 || next.second >= W)
			continue;
		findNumber(currDir, dir, next, path);
	}
}

int sumNumber()
{
	int ret = 0;

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			if (map[h][w]) {
				string path(1, '0');
				findNumber(0, 0, make_pair(h, w), path);
				int number = calcNumber(path);
				ret += number;
			}
		}
	}

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
	generateNumPaths();
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		int ret = sumNumber();
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