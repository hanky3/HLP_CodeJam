#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#if 0
using namespace std;

#define MAX_BOARD_SIZE 21
#define MAX_INF 999999999
int sCount;
int k;
vector<int> S;
vector<int> cache;
vector<int> countCache;

FILE *fpInput, *fpOutput;

int getMaxLISLength(int startIndex)
{
	int &ret = cache[startIndex+1];

	if (ret != -1)
		return ret;
	
	ret = 1;
	for (int index = startIndex+1; index < (int)S.size(); index++)
	{
		if (startIndex == -1 || S[startIndex] < S[index])
			ret = max(ret, getMaxLISLength(index) + 1);
	}

	return ret;
}

int getLISCount(int startIndex)
{
	int &ret = countCache[startIndex + 1];
	if (ret != -1)
		return ret;

	if (getMaxLISLength(startIndex) == 1)
	{
		ret = 1;
		return ret;
	}

	int currentNum = -1;
	if (startIndex >= 0)
		currentNum = S[startIndex];

	ret = 0;
	for (int index = startIndex + 1; index < (int)S.size(); index++)
	{
		if ( (currentNum < S[index]) && (getMaxLISLength(startIndex) == getMaxLISLength(index) + 1))
			ret += getLISCount(index);
	}

	return ret;
}

int skip;
bool compare(int i, int j) { return S[i] < S[j]; }
bool reconstruct(vector<int> &result, int startIndex, int k)
{
	int remainCount = getLISCount(startIndex);
	if ( (startIndex >= 0) && (skip + remainCount < k) )
	{
		skip += remainCount;
		return false;
	}

	if (getMaxLISLength(startIndex) == 1)
	{
		skip++;
		if (skip == k)
			return true;
	}

	vector <int> candidateList;
	int currentNum = -1;
	if (startIndex >= 0)
		currentNum = S[startIndex];

	for (int index = startIndex + 1; index < (int)S.size(); index++)
	{
		if ((currentNum < S[index]) && (getMaxLISLength(startIndex) == getMaxLISLength(index) + 1) )
			candidateList.push_back(index);
	}
	sort(candidateList.begin(), candidateList.end(), compare);

	for (auto index : candidateList)
	{
		result.push_back(S[index]);
		if (reconstruct(result, index, k))
			return true;
		result.pop_back();
	}
	return false;
}


void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");

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

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++)
	{
		int k;
		fscanf(fpInput, "%d %d", &sCount, &k);
		S.clear();
		cache.clear();
		countCache.clear();

		S.resize(sCount);
		cache.resize(sCount+1, -1);
		countCache.resize(sCount+1, -1);

		for (int i = 0; i < sCount; i++)
		{
			fscanf(fpInput, "%d", &S[i]);
		}

		int ret = getMaxLISLength(-1) - 1;
		int lisCount = getLISCount(-1);
		vector<int> output;
		skip = 0;
		reconstruct(output, -1, k);

		fprintf(fpOutput, "%d\n", ret);
		for (auto number : output)
			fprintf(fpOutput, "%d ", number);
		fprintf(fpOutput, "\n");

		if (isFile)
		{
			printf("%d\n", ret);
			for (auto number : output)
				printf("%d ", number);
			printf("\n");
		}
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif