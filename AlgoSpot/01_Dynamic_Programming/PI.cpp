#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <math.h>
#include <assert.h>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_DIFF_LEVEL		(100)
#define MAX_NUM_LENGTH	(10001)

string numberStr;
int cache[MAX_NUM_LENGTH];

void readInputData()
{
	char buf[MAX_NUM_LENGTH];
	fscanf(fpInput, "%s", buf);

	numberStr = string(buf);
	memset(cache, -1, sizeof(cache));
}

bool checkNumPriority1(int start, int end)
{
	for (int index = start + 1; index <= end; index++) {
		if (numberStr[index] != numberStr[start]) {
			return false;
		}
	}
	return true;
}

int checkNumPriority2(int start, int end)
{
	bool isIncrease = true;

	if (numberStr[start] - '0' > numberStr[start + 1] - '0')
		isIncrease = false;

	int nextNum = (numberStr[start] - '0');
	for (int index = start; index <= end; index++) {
		int currNum = numberStr[index] - '0';
		if (currNum != nextNum)
			return false;
		if (isIncrease) nextNum++;
		else nextNum--;
	}
	return true;
}

int checkNumPriority4(int start, int end)
{
	int nums[2];
	nums[0] = (numberStr[start] - '0');
	nums[1] = (numberStr[start+1] - '0');

	int oddIndex = 0;
	for (int index = start; index <= end; index++) {
		int currNum = numberStr[index] - '0';
		if (currNum != nums[oddIndex % 2])
			return false;
		oddIndex++;
	}

	return true;
}

bool checkNumPriority5(int start, int end)
{
	int interval = 0;
	interval = (numberStr[start + 1] - '0') - (numberStr[start] - '0');

	int prevNum = numberStr[start] - '0';
	for (int index = start; index <= end; index++) {
		int currNum = numberStr[index] - '0';
		if (currNum != prevNum)	return false;
		prevNum += interval;
	}

	return true;
}

int minPrioritySum(int start, int end)
{
	if (checkNumPriority1(start, end))
		return 1;
	if (checkNumPriority2(start, end))
		return 2;
	if (checkNumPriority4(start, end))
		return 4;
	if (checkNumPriority5(start, end))
		return 5;

	return 10;
}

int minDifficultyLevel(int start)
{
	int &ret = cache[start];
	if (ret != -1)
		return ret;

	int remainSize = numberStr.size() - start;
	if (remainSize < 3)
		return MAX_DIFF_LEVEL;
	else if (remainSize >= 3 && remainSize <= 5)
		return minPrioritySum(start, numberStr.size() - 1);

	ret = numeric_limits<int>::max();

	for (int end = start + 2; end <= start + 4; end++) {
		int priority = minPrioritySum(start, end);
		ret = min(ret, priority + minDifficultyLevel(end + 1));
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

	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		int ret = minDifficultyLevel(0);
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