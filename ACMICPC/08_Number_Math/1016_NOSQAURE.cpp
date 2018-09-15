#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

long long minNum, maxNum;

void readInputData()
{
	fscanf(fpInput, "%lld %lld", &minNum, &maxNum);
}

int noSqrtDividedNums()
{
	int diff = (int)(maxNum - minNum) + 1;
	vector<bool> dividedNums(diff, false);

	int maxSqrtN = (int)sqrt(maxNum);
	for (int n = 2; n <= maxSqrtN; n++) {
		long long sqrtN = ((long long)n*n);
		long long start = (minNum / sqrtN) * sqrtN;
		if (minNum % sqrtN > 0) start += sqrtN;
		while (start <= maxNum) {
			dividedNums[start - minNum] = true;
			start += sqrtN;
		}
	}

	int count = 0;
	for (auto divided : dividedNums) {
		if (!divided)
			count++;
	}
	return count;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	fpOutput = fopen("output.txt", "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = noSqrtDividedNums();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}