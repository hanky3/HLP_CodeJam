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

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	numbers = vector<int>(N);
	for (int i = 0; i < N; i++)
		fscanf(fpInput, "%d", &numbers[i]);
}

int maxContinueSum()
{
	int ret = numeric_limits<int>::min();
	int sum = 0;

	for (auto n : numbers) {
		sum += n;
		ret = max(ret, sum);
		if (sum < 0)
			sum = 0;
	}

	return ret;
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
		auto ret = maxContinueSum();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
