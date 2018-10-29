#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;
typedef unsigned long long uint64;

FILE *fpInput;
FILE *fpOutput;

#define MOD	1000000007

int A, B, N;

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &A, &B, &N);

}

uint64 powerVal(int num, int digit)
{
	uint64 val = 0;
	if (digit == 1 || num == 1)
		return num;

	int mid = digit / 2;
	val = powerVal(num, mid);
	val *= val;
	val %= MOD;
	if (digit % 2)
		val *= num;
	val %= MOD;

	return val;
}

uint64 passwordCount(int start, int end)
{
	uint64 ret = 0;

	if (start == end) {
		ret = powerVal(N, start);
		return ret;
	}

	int mid = (start + end) / 2;
	bool remains = ((start + end) % 2 == 0);
	if (remains)
		mid--;
	int diff = (end - start + 1);
	uint64 left = passwordCount(start, mid);
	uint64 right = left * powerVal(N, mid-start+1);
	ret = left + right;
	if (remains)
		ret += powerVal(N, end);
	ret %= MOD;

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

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		long long ret = passwordCount(A, B);
		fprintf(fpOutput, "%lld\n", ret);
		if (isFile)
			printf("%lld\n", ret);

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