#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

#define MAX_NUMBER_SIZE	100001

FILE *fpInput;
FILE *fpOutput;

int N;
vector<int> numbers;
int cache[MAX_NUMBER_SIZE][2];

void readInputData()
{
	memset(cache, -10, sizeof(cache));

	fscanf(fpInput, "%d", &N);
	numbers = vector<int>(N);
	for (int i = 0; i < N; i++)
		fscanf(fpInput, "%d", &numbers[i]);
}

int maxContinueSum(int start, bool continues)
{
	int &ret = cache[start][continues];
	if (ret >= -1000)
		return ret;
	if (start >= N)	return 0;
	if (start == N - 1 && !continues)
		return numbers[start];

	ret = numbers[start];
	ret = max(ret, numbers[start] + maxContinueSum(start + 1, true));
	if (!continues) {
		ret = max(ret, maxContinueSum(start + 1, false));
	}

	return ret;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	string outputFileName = string(argv[1]);
	outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
	fpOutput = fopen(outputFileName.c_str(), "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = maxContinueSum(0, false);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
