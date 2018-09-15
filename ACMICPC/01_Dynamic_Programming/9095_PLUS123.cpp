#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_N		20

int N;
int dp[MAX_N];

void readInputData()
{
	memset(dp, 0, sizeof(dp));
	fscanf(fpInput, "%d", &N);
}

int waysOfNum(int num)
{
	dp[1] = 1;
	dp[2] = 1;
	dp[3] = 1;

	for (int n = 1; n <= num; n++) {
		for (int sub = 1; sub <= 3; sub++) {
			if (n - sub > 0)
				dp[n] += dp[n - sub];
		}
	}
	return dp[num];
}

void solveProblem(const char *fileName, bool isFile)
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(fileName, "r");
	string outputFileName = string(fileName);
	outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
	fpOutput = fopen(outputFileName.c_str(), "w");
#endif

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		auto ret = waysOfNum(N);
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
}

int main(int argc, char* argv[])
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif
	return 0;
}
