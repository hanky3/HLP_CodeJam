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

#define MAX_PRODUCT_SIZE		(1001)

FILE *fpInput;
FILE *fpOutput;

int materialCount;
vector<vector<int>> materialItems;
vector<int> materialNeeds;


void readInputData()
{
	fscanf(fpInput, "%d", &materialCount);

	materialItems = vector<vector<int>>(materialCount);
	materialNeeds = vector<int>(materialCount, 0);

	for (int i = 0; i < materialCount; i++) {
		int itemCount;
		fscanf(fpInput, "%d %d", &materialNeeds[i], &itemCount);

		materialItems[i] = vector<int>(itemCount);
		for (int j = 0; j < itemCount; j++) {
			fscanf(fpInput, "%d", &materialItems[i][j]);
		}
		sort(materialItems[i].begin(), materialItems[i].end(), greater<int>());
	}
}

int maxProductCount()
{
	int ret = 0;
	int selectCnt = 10;

	vector<int> materialSums = vector<int>(materialCount, 0);
	while (selectCnt > 0) {
		bool selected = false;
		int selMaterial = -1;
		int nextSum = 0;

		for (int index = 0; index < materialNeeds.size(); index++) {
			if (selMaterial < 0 && materialItems[index].size() > 0) {
				selMaterial = index;
				nextSum = materialItems[index][0];
				continue;
			}

			if (!materialItems[index].empty()) {
				int selected = materialSums[selMaterial] / materialNeeds[selMaterial];
				int curr = materialSums[index] / materialNeeds[index];

				if (curr < selected) {
					selMaterial = index;
					nextSum = materialItems[index][0];
				}
			}
		}

		if (selMaterial < 0)
			break;

		materialSums[selMaterial] += nextSum;
		materialItems[selMaterial].erase(materialItems[selMaterial].begin());
		selectCnt--;
	}

	ret = numeric_limits<int>::max();
	for (int index = 0; index < materialSums.size(); index++) {
		int count = materialSums[index] / materialNeeds[index];
		ret = min(ret, count);
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

		int ret =	maxProductCount();
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