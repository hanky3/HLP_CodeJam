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

#define MAX_NUMBER_CNT		110
#define MAX_QUANTIZE			11	

int numberCnt;
int quantizeCnt;
vector<int> numbers;
int cache[MAX_NUMBER_CNT][MAX_QUANTIZE];

void readInputData()
{
	fscanf(fpInput, "%d %d", &numberCnt, &quantizeCnt);

	numbers = vector<int>(numberCnt);
	for (int index = 0; index < numberCnt; index++) {
		fscanf(fpInput, "%d", &numbers[index]);
	}
	memset(cache, -1, sizeof(cache));
	if (quantizeCnt > numberCnt)
		quantizeCnt = numberCnt;

	sort(numbers.begin(), numbers.end());
}

int quantizeSum(int start, int end)
{
	int total = 0, average = 0;
	int sum;

	if (start > end)
		return 0;

	for (int index = start; index <= end; index++)
		total += numbers[index];

	average = total / (double)(end - start + 1) + 0.5;

	sum = 0;
	for (int index = start; index <= end; index++)
		sum += pow<int>((numbers[index]-average), 2);

	return sum;
}

int minQuantization(int start, int q)
{
	int &ret = cache[start][q];

	if (ret != -1)
		return ret;

	if (q == 1) {
		ret = quantizeSum(start, numbers.size() - 1);
		return ret;
	}

	ret = numeric_limits<int>::max();
	for (int end = start; end < (numbers.size() - q + 1); end++) {
		int sum = quantizeSum(start, end);
		ret = min(ret, sum + minQuantization(end + 1, q - 1));
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
		int ret = minQuantization(0, quantizeCnt);
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