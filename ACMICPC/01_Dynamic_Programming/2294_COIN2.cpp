#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

#define MAX_COIN_COUNT	101
#define MAX_PRICE					10001
#define INF								987654321

FILE *fpInput;
FILE *fpOutput;


int n, k;
vector<int> coinPrices;
int dp[MAX_COIN_COUNT][MAX_PRICE];

void readInputData()
{
	fscanf(fpInput, "%d %d", &n, &k);
	memset(dp, -1, sizeof(dp));

	coinPrices = vector<int>(n, 0);
	for (int i = 0; i < n; i++) {
		fscanf(fpInput, "%d", &coinPrices[i]);
	}
}

int minCoinUsed(int currCoin, int sum)
{
	int &ret = dp[currCoin][sum];
	if (ret != -1)
		return ret;

	if (sum == k)
		return 0;

	if (currCoin >= n)
		return INF;

	ret = INF;
	int coinCount = 0;
	while (sum <= k) {
		ret = min(ret, coinCount + minCoinUsed(currCoin + 1, sum));
		sum += coinPrices[currCoin];
		coinCount++;
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
		auto ret = minCoinUsed(0, 0);
		if (ret == INF)	ret = -1;
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
