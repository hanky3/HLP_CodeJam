#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

#define MAX_DIGITS	((1 << 14) - 1)
#define MAX_M		(20 + 1)
#define MOD			(1000000007)

int cache[MAX_M][MAX_DIGITS][2];
unsigned long long E;
int M;
vector<int> digitNums;

void readInputData()
{
	fscanf(fpInput, "%llu %d", &E, &M);

	digitNums.clear();
	unsigned long long temp = E;
	while (temp > 0)
	{
		digitNums.push_back(temp % 10);
		temp /= 10;
	}
	memset(cache, -1, sizeof(cache));
}

int priceCount(int remains, int usageBits, bool compareFlag)
{
	int &ret = cache[remains][usageBits][compareFlag];
	if (ret != -1)
		return ret;

	int allFlags = (1 << digitNums.size()) - 1;
	if (usageBits == allFlags)
	{
		if (compareFlag || remains)
			return 0;

		return 1;
	}

	ret = 0;
#ifdef WIN32
	int pos = digitNums.size() - (__popcnt(usageBits) + 1);
#else
	int pos = digitNums.size() - (__builtin_popcount(usageBits) + 1); 
#endif

	int usageNumBits = 0;
	for (int index = 0; index < (int)digitNums.size(); index++)
	{
		int num = digitNums[index];

		if (usageBits & (1 << index))
			continue;

		if (usageNumBits & (1 << num))
			continue;

		bool newCompare = false;
		if (compareFlag)
		{
			if (num > digitNums[pos])
				continue;
			if (num == digitNums[pos])
				newCompare = true;
		}

		int remainNext = (num * (unsigned long long)pow(10,pos)) % M;
		remainNext += remains;
		remainNext %= M;

		usageNumBits |= (1 << num);

		usageBits |= (1 << index);
		ret += priceCount(remainNext, usageBits, newCompare);
		ret %= MOD;
		usageBits &= ~(1 << index);
	}

	return ret;
}

void solveProblem(const char *fileName, bool isFile)
{
	if (isFile)
	{
		fpInput = fopen(fileName, "r");

		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 3) + "out";
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
		int ret = priceCount(0, 0, 1);

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
