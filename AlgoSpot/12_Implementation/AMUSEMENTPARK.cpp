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

int N, M, S;
vector<vector<bool>> remainLineMap;
vector<pair<int, int>> lineInfo;


bool compare(pair<double, int> &a, pair<double, int> &b) {
	return (a.first > b.first);
}

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &N, &M, &S);
	remainLineMap = vector<vector<bool>>(N, vector<bool>(M, false));
	lineInfo = vector<pair<int, int>>(N*M, make_pair(-1, -1));

	for (int y = 0; y < N; y++) {
		for (int x = 0; x < M; x++) {
			int num;
			fscanf(fpInput, "%d", &num);
			if (num == 0)
				continue;

			remainLineMap[y][x] = true;
			lineInfo[num - 1] = make_pair(y, x);
		}
	}

	auto itr = find(lineInfo.begin(), lineInfo.end(), make_pair(-1, -1));
	lineInfo.erase(itr, lineInfo.end());
}

bool canWatchMelon(int melonNum)
{
	pair<int, int> start = lineInfo.front();
	pair<int, int> end = lineInfo[melonNum];

	if (start.first > end.first)
		std::swap(start, end);

	int diffY = end.first - start.first;
	int diffX = end.second - start.second;
	int currX = start.second;

	if (diffY == 0) {
		if (start.second > end.second)
			std::swap(start, end);

		for (int next = 1; next < abs(diffX); next++) {
			int nextX = start.second + next;
			if (remainLineMap[start.first][nextX])
				return false;
		}
		return true;
	}

	for (int next = 1; next < abs(diffY); next++) {
		if ((abs(diffX) * next) % diffY != 0)
			continue;

		int nextX = start.second + (diffX * next / diffY);
		if (remainLineMap[start.first + next][nextX])
			return false;
	}
	return true;
}

vector<int> numOfMelonSeen()
{
	vector<int> ret;

	int index = 1;
	while (lineInfo.size() >= S) {
		if (canWatchMelon(S - 1))
			ret.push_back(index);
		lineInfo.erase(lineInfo.begin());
		index++;
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

	int testCase = 1;

	//fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		vector<int> ret = numOfMelonSeen();
		if (isFile)
			printf("%d\n", ret.size());
		fprintf(fpOutput, "%d\n", ret.size());

		for (auto num : ret) {
			if (isFile)
				printf("%d\n", num);
			fprintf(fpOutput, "%d\n", num);
		}

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