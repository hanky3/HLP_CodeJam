#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <functional>

using namespace std;

#define MAX_CLIMB_LENGTH	(1001)
#define MAX_RAIN_DAYS		(1001)

FILE *fpInput;
FILE *fpOutput;

int studentCnt, friendsCnt;
vector<vector<int>> friendsInfo;

void readInputData()
{
	fscanf(fpInput, "%d %d", &studentCnt, &friendsCnt);

	friendsInfo = vector<vector<int>>(studentCnt);
	for (int i = 0; i < friendsCnt; i++) {
		pair<int, int> relations;
		fscanf(fpInput, "%d %d", &relations.first, &relations.second);
		friendsInfo[relations.first].push_back(relations.second);
		friendsInfo[relations.second].push_back(relations.first);
	}

}

int findFriends(vector<int> &selected, bool isPairExist)
{
	bool allSelected = true;
	int nextFriend = 0;
	for (int i = 0; i < selected.size(); i++) {
		if (!selected[i]) {
			allSelected = false;
			nextFriend = i;
			break;
		}
	}

	if (allSelected)
		return isPairExist ? 1 : 0;

	int ret = 0;
	selected[nextFriend] = true;
	for (auto myFriend : friendsInfo[nextFriend]) {
		if (selected[myFriend])
			continue;

		selected[myFriend] = true;
		ret += findFriends(selected, true);
		selected[myFriend] = false;
	}
	//ret += findFriends(selected, isPairExist);
	selected[nextFriend] = false;

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

		vector<int> selected(studentCnt, 0);
		int ret = findFriends(selected, false);
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