#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;
typedef long long int64;

int N;
vector<int> budgetList;
int totalBudget;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	budgetList = vector<int>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &budgetList[index]);
	}
	fscanf(fpInput, "%d", &totalBudget);
}

bool satisfyBudgetSum(int budget, int total)
{
	int needed = 0;
	for (auto b : budgetList) {
		needed += min(budget, b);
	}
	return (needed <= total);
}


int maxBudgetPrice()
{
	int lo, hi, mid;
	lo = 1;
	hi = totalBudget + 1;

	while (lo + 1 < hi) {
		mid = (lo + hi) / 2;
		if (satisfyBudgetSum(mid, totalBudget))
			lo = mid;
		else
			hi = mid;
	}

	int ret = 0;
	for (auto b : budgetList)
		ret = max(ret, min(lo, b));

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
		auto ret = maxBudgetPrice();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

