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

#define MAX_TRIANGLE_SIZE		(101)

FILE *fpInput;
FILE *fpOutput;

int n;
vector<vector<int>> triangleNums;
int cache[MAX_TRIANGLE_SIZE][MAX_TRIANGLE_SIZE];

void readInputData()
{
	fscanf(fpInput, "%d", &n);

	triangleNums = vector<vector<int>>(n);
	for (int i = 0; i < n; i++) {
		triangleNums[i] = vector<int>(i + 1);
		for (int j = 0; j <= i; j++) {
			fscanf(fpInput, "%d", &triangleNums[i][j]);
		}
	}

	memset(cache, -1, sizeof(cache));
}

int maxTrianglePath(int x, int y)
{
	int &ret = cache[x][y];

	if (ret != -1)
		return ret;

	if (y >= n || x > y)
		return 0;

	ret = 0;
	ret = max(ret, triangleNums[y][x] + maxTrianglePath(x, y + 1));
	ret = max(ret, triangleNums[y][x] + maxTrianglePath(x + 1, y + 1));

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

		int ret = maxTrianglePath(0, 0);
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