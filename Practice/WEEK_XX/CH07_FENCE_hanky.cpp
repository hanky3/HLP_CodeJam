#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

#if 0
using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_HEIGHT 99999
int fenceNum;
int maxHeight;
int cacheRectSize;
vector<int> fenceHeights;

void readInputData()
{
	fscanf(fpInput, "%d", &fenceNum);
	fenceHeights = vector<int>(fenceNum, 0);
	maxHeight = 0;
	cacheRectSize = 0;

	for (int index = 0; index < fenceNum; index++) {
		fscanf(fpInput, "%d", &fenceHeights[index]);
		maxHeight = max(maxHeight, fenceHeights[index]);
	}
}

int maxMidRectArea(int start, int end)
{
	int rectArea = 0;

	int currHeight = 0;
	int mid = (start + end) / 2;
	int left = mid;
	int right = mid + 1;

	int minHeight = min(fenceHeights[left], fenceHeights[right]);
	int nextIndex = left;
	rectArea = minHeight * 2;
	while (true)
	{
		// area
		minHeight = min(minHeight, fenceHeights[nextIndex]);
		rectArea = max(rectArea, minHeight*(right - left + 1));

		if (left <= start && right >= end)
			break;

		int nextLeft = left - 1;
		int nextRight = right + 1;

		if ((nextLeft < start) || 
			((nextRight <= end) && (fenceHeights[nextLeft] < fenceHeights[nextRight])) ) {
			right++;
			nextIndex = right;
		}
		else {
			left--;
			nextIndex = left;
		}
	}

	return rectArea;
}


int maxFenceArea(int start, int end)
{
	if (start == end)
		return fenceHeights[start];

	int maxHeuristicSize = (end - start + 1) * maxHeight;
	if (maxHeuristicSize < cacheRectSize)
		return 0;

	int ret = 0;
	int mid = (start + end) / 2;

	int leftArea = maxFenceArea(start, mid);
	int rightArea = maxFenceArea(mid+1, end);
	ret = max(leftArea, rightArea);
	ret = max(ret, maxMidRectArea(start, end));
	cacheRectSize = max(cacheRectSize, ret);

	return ret;
}

void solveProblem(const char *fileName, bool isFile)
{
	if (isFile)
	{
		fpInput = fopen(fileName, "r");
		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}
	else
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		int ret = maxFenceArea(0, fenceHeights.size()-1);
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
#endif