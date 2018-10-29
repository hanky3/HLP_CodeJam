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

int numberCnt;
vector<int> numMoves;

void readInputData()
{
	fscanf(fpInput, "%d", &numberCnt);
	numMoves = vector<int>(numberCnt, 0);
	for (int index = 0; index < numberCnt; index++) {
		fscanf(fpInput, "%d", &numMoves[index]);
	}
}

vector<int> generateNumbers()
{
	vector<int> ret;
	vector<int> numbers = vector<int>(numberCnt);
	for (int index = 0; index < numberCnt; index++)
		numbers[index] = index + 1;

	auto reverseItr = numMoves.rbegin();
	for (; reverseItr != numMoves.rend(); reverseItr++) {
		int move = *reverseItr;

		int pos = numbers.size() - move - 1;
		ret.push_back(numbers[pos]);
		numbers.erase(numbers.begin() + pos);
	}
	reverse(ret.begin(), ret.end());

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
		vector<int> ret = generateNumbers();
		if (isFile) {
			for (auto num : ret)	
				printf("%d ", num);
			printf("\n");
		}
		for (auto num : ret)
			fprintf(fpOutput, "%d ", num);
		fprintf(fpOutput, "\n");
		//fprintf(fpOutput, "%s\n", ret.c_str());

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