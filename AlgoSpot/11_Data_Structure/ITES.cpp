#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

using namespace std;
#define INF 0x0FFFFFFF
#define MAX_V 1001

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

int K, N;

unsigned long long previousNum;
int randomGenerator(int index)
{
	unsigned long long mod = pow(2, 32);
	if (index == 0)
	{
		previousNum = 1983;
		return (int)previousNum;
	}

	previousNum = ((previousNum%mod) * 214013 + 2531011) % mod;
	return (previousNum % 10000) + 1;
}

int numOfPartialSum()
{
	int currentSum = 0;
	int partialSumCnt = 0;
	queue<int> numberList;

	for (int index = 0; index < N; index++)
	{
		int signal = randomGenerator(index);
		numberList.push(signal);
		currentSum += signal;

		while (currentSum >= K)
		{
			if (currentSum == K) partialSumCnt++;
			currentSum -= numberList.front();
			numberList.pop();
		}
	}

	return partialSumCnt;
}

// Solve the problem(read the input file / and retrieves the result
void solveProblem(char *filename, bool isFile)
{
	//	gFpOutput = fopen("Test.txt", "w");
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

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);

	for (int i = 0; i < testCase; i++)
	{
		fscanf(fpInput, "%d %d\n", &K, &N);

		// result calculation 
		int ret = numOfPartialSum();
		fprintf(fpOutput, "%d", ret);
		if (isFile)
			printf("%d", ret);

		fprintf(fpOutput, "\n");
		if (isFile)
			printf("\n");

	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	//	fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif

	return 0;
}