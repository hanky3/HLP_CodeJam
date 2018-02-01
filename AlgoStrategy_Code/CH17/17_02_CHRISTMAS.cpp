#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>

#if 0
using namespace std;

const double PI = 2.0 * acos(0.0);
FILE *fpInput = NULL;
FILE *fpOutput = NULL;

vector<int> getPartialSum(vector<int> input, int k)
{
	vector<int> partialSum(input.size(), 0);
	partialSum[0] = input[0] % k;
	for (int i = 1; i < input.size(); i++)
	{
		partialSum[i] = (partialSum[i - 1] + input[i]) % k;
	}
	return partialSum;
}

int numOfBuys(vector<int> psum, int k)
{
	const int MOD = 20091101;
	int ret = 0;

	vector<int> count(k, 0);
	for (int index = 0; index < psum.size(); index++)
	{
		count[psum[index]]++;
	}

	for (int index = 0; index < k; index++)
	{
		if (count[index] > 1)
		{
			ret = (ret + (count[index] * (count[index] - 1) / 2)) % MOD;
		}
	}
	return ret;
}

int maxBuys(vector<int> psum, int n, int k)
{
	int ret = 0;

	vector<int> prevBuys(n, -1);
	for (int index = 0; index < psum.size(); index++)
	{
		for (int j = 0; j < psum.size(); j++)
		ret = max(ret, 0);
	}

	return ret;
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
		int N = 0, K = 0;
		fscanf(fpInput, "%d %d", &N, &K);

		vector<int> boxList(N, 0);
		for (int j = 0; j < N; j++)
		{
			fscanf(fpInput, "%d", &boxList[j]);
		}

		vector<int> psum = getPartialSum(boxList, K);
		int numOfCase = numOfBuys(psum, K);
		int maxBuyCase = maxBuys(psum, N, K);

		fprintf(fpOutput, "%d %d\n", numOfCase, maxBuyCase);
		if (isFile)
			printf("%d %d", output.c_str());
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