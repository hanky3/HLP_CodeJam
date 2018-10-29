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

#define MAX_LIS_SIZE		110

int aLength, bLength;
vector<int> numberA;
vector<int> numberB;
int cache[MAX_LIS_SIZE][MAX_LIS_SIZE];

void readInputData()
{
	fscanf(fpInput, "%d %d", &aLength, &bLength);
	memset(cache, -1, sizeof(cache));

	numberA = vector<int>(aLength, 0);
	numberB = vector<int>(bLength, 0);

	memset(cache, -1, sizeof(cache));

	for (int index = 0; index < aLength; index++)
		fscanf(fpInput, "%d", &numberA[index]);

	for (int index = 0; index < bLength; index++)
		fscanf(fpInput, "%d", &numberB[index]);

}

int maxJointLisLength(int startA, int startB)
{
	int &ret = cache[startA + 1][startB + 1];
	if (ret != -1)
		return ret;

	int curr = numeric_limits<int>::min();
	if (startA >= 0) {
		curr = numberA[startA];
	}
	if (startB >= 0) {
		curr = max<int>(curr, numberB[startB]);
	}

	ret = 2;
	for (int nextA = startA + 1; nextA < aLength; nextA++) {
		//int selected = (startA == -1) ? 0 : 1;
		if (numberA[nextA] > curr) {
			ret = max(ret, 1 + maxJointLisLength(nextA, startB));
		}
	}

	for (int nextB = startB + 1; nextB < bLength; nextB++) {
		//int selected = (startB == -1) ? 0 : 1;
		if (numberB[nextB] > curr)
			ret = max(ret, 1 + maxJointLisLength(startA, nextB));
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
		int ret = maxJointLisLength(-1, -1);
		ret -= 2;
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