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

#define MAX_FRIEND_NAME_SIZE		11

int friendCnt, foodCnt;
map<string, int> friendIDs;
vector<unsigned long long> foodPref;
int cacheFoodCnt;

void readInputData()
{
	fscanf(fpInput, "%d %d", &friendCnt, &foodCnt);

	friendIDs.clear();
	foodPref = vector<unsigned long long>(foodCnt, 0);

	char buf[MAX_FRIEND_NAME_SIZE];
	for (int index = 0; index < friendCnt; index++) {
		fscanf(fpInput, "%s", buf);
		friendIDs[string(buf)] = index;
	}

	for (int food = 0; food < foodCnt; food++) {
		int foodFriends;
		fscanf(fpInput, "%d", &foodFriends);

		while (foodFriends > 0) {
			fscanf(fpInput, "%s", buf);
			foodPref[food] |= (1ull << friendIDs[string(buf)]);
			foodFriends--;
		}
	}

	cacheFoodCnt = numeric_limits<int>::max();
}

int minFoodCooking(int step, unsigned long long preference, int sum)
{
	unsigned long long prefAll = (1ull << friendCnt) - 1;
	if ((preference & prefAll) == prefAll) {
		cacheFoodCnt = min(cacheFoodCnt, sum);
		return sum;
	}

	if (step == foodCnt) {
		return foodCnt;
	}

	if (sum >= cacheFoodCnt)
		return cacheFoodCnt;

	int ret = numeric_limits<int>::max();
	unsigned long long newPref = (preference | foodPref[step]);

	if (preference != newPref)
		ret = min(ret, minFoodCooking(step + 1, newPref, sum+1));
	ret = min(ret, minFoodCooking(step + 1, preference, sum));

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
		int ret = minFoodCooking(0, 0ULL, 0);
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