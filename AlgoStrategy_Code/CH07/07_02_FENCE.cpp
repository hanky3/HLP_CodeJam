#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#if 0

//#define MOD 1000000007
#define MAX_NUM 20000
int fenceNum;
//int cache[MAX_NUM][MAX_NUM];
int fenceHeight[MAX_NUM];

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

int getMaxMiddleRectangleSize(int startIdx, int endIdx)
{
	int maxSize = 0;
	int mid = (startIdx + endIdx) / 2;

	int startIndex;
	if (fenceHeight[mid] > fenceHeight[mid + 1])
		startIndex = mid;
	else
		startIndex = mid + 1;

	int leftStep = startIndex;
	int rightStep = startIndex;

	maxSize = fenceHeight[startIndex];
	int select = 1;
	int currHeight = fenceHeight[startIndex];
	while (leftStep > 0 || rightStep < endIdx)
	{
		select++;
		if (leftStep == 0 || (rightStep < endIdx && fenceHeight[leftStep - 1] <= fenceHeight[rightStep + 1]) )
		{
			rightStep++;
			currHeight = min(currHeight, fenceHeight[rightStep]);
		}
		else
		{
			leftStep--;
			currHeight = min(currHeight, fenceHeight[leftStep]);
		}

		maxSize = max(maxSize, select*currHeight);
	}

	return maxSize;
}

int maxSize;
int getMaxRectangleSize(int fenceNum, int startIdx, int endIdx)
{
	/*
	if (maxSize > 0)
		return maxSize;
	*/

	if (startIdx == endIdx)
		return fenceHeight[startIdx];

	int maxHeight = 0;
	for (int index = startIdx; index <= endIdx; index++)
		maxHeight = max(maxHeight, fenceHeight[index]);

	int heuristicSize = maxHeight*(endIdx - startIdx + 1);
	if (heuristicSize < maxSize)
		return 0;

	int mid = (startIdx + endIdx) / 2;
	int left = getMaxRectangleSize(fenceNum, startIdx, mid);
	int right = getMaxRectangleSize(fenceNum, mid+1, endIdx);

	maxSize = max(left, right);
	int midSize = getMaxMiddleRectangleSize(startIdx, endIdx);
	maxSize = max(maxSize, midSize);
	// mid level;
	return maxSize;
}

void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");

	if (!isFile)
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else
	{
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++)
	{
		//memset(cache, -1, MAX_NUM*MAX_NUM*sizeof(int));
		fscanf(fpInput, "%d", &fenceNum);

		for (int i = 0; i < fenceNum; i++)
		{
			fscanf(fpInput, "%d", &fenceHeight[i]);
		}

		maxSize = -1;
		getMaxRectangleSize(fenceNum, 0, fenceNum-1);
		fprintf(fpOutput, "%d\n", maxSize);
		if (isFile)
			printf("%d\n", maxSize);
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif