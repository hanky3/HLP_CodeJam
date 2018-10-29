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

#define MAX_K			(100000 + 1)
#define MOD			(20091101)

int N, K;
vector<int> puppets;
vector<pair<int, int>> psum;
int cache[MAX_K];

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &K);
	puppets = vector<int>(N);

	for (int i = 0; i < N; i++)
		fscanf(fpInput, "%d", &puppets[i]);

	memset(cache, -1, sizeof(cache));
}

void makePartialSum(vector<int> &input, int mod)
{
	psum = vector<pair<int, int>>(input.size() + 1);
	psum[0] = make_pair(0, 0);

	for (int i = 1; i <= (int)input.size(); i++)
	{
		int sum = psum[i - 1].second + input[i - 1];
		sum %= mod;
		psum[i] = make_pair(i, sum);
	}
}

int orderCount()
{
	int ret = 0;

	vector<unsigned long long> psumCount(K, 0);
	for (auto val : psum)
		psumCount[val.second]++;

	for (auto val : psumCount)
	{
		unsigned long long numOfCase = val * (val - 1) / 2;
		numOfCase %= MOD;
		ret += numOfCase;
		ret %= MOD;
	}

	return ret;
}

vector<vector<int>> psumPos;
void calculatePsumPos()
{
	psumPos = vector<vector<int>>(K);
	for (auto val : psum)
		psumPos[val.second].push_back(val.first);
}

int maxOrderCount(int start)
{
	if (start >= psum.size())
		return 0;

	int &ret = cache[start];
	if (ret != -1)
		return ret;

	ret = 0;
	auto &posList = psumPos[psum[start-1].second];
	int nextPos = -1;
	auto findItr = lower_bound(posList.begin(), posList.end(), start);
	if (findItr != posList.end())
		nextPos = *findItr;
	/*
	for (auto pos : posList)
	{
		if (pos >= start) {
			nextPos = pos;
			break;
		}
	}
	*/
	if (nextPos > -1)
		ret = 1 + maxOrderCount(nextPos + 1);

	ret = max(ret, maxOrderCount(start + 1));

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
		makePartialSum(puppets, K);
		int ret = orderCount();

		calculatePsumPos();
		int maxOrder = maxOrderCount(1);

		if (isFile)
			printf("%d %d\n", ret, maxOrder);
		fprintf(fpOutput, "%d %d\n", ret, maxOrder);

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