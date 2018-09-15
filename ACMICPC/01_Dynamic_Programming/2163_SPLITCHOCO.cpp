#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

FILE *fpInput;
FILE *fpOutput;
int N, M;
int dp[301][301];

void readInputData()
{
	memset(dp, -1, sizeof(dp));
	fscanf(fpInput, "%d %d", &N, &M);
}

int splitCount(int n, int m)
{
	int &ret = dp[n][m];
	if (ret != -1)	return ret;

	if (n == 1 && m == 1)
		return 0;

	ret = numeric_limits<int>::max();
	for (int nextN = 1; nextN <= (n/2); nextN++)
		ret = min(ret, 1 + (splitCount(nextN, m) + splitCount(n - nextN, m)));
	for (int nextM = 1; nextM <= (m/2); nextM++)
		ret = min(ret, 1 + (splitCount(n, nextM) + splitCount(n, m-nextM)));
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
	while (testCase > 0) {
		readInputData();
		auto ret = splitCount(N, M);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);

	return 0;
}