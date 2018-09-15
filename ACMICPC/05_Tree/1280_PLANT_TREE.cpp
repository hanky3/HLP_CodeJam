#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

#define MOD		1000000007

int N;
vector<int> treePosList;

struct FenwickTree {
	vector<long long> trees;

	FenwickTree(int n) : trees(n + 1, 0) {};
	void add(int pos, long long val) {
		while (pos < trees.size()) {
			trees[pos] += val;
			pos += (pos & -pos);
		}
	}

	long long sum(int pos) {
		long long ret = 0;
		while (pos > 0) {
			ret += trees[pos];
			pos &= (pos - 1);
		}
		return ret;
	}
};

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	treePosList = vector<int>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &treePosList[index]);
	}
}

long long plantTreeCost()
{
	long long ret = 1;
	FenwickTree countQ(200000);
	FenwickTree distanceQ(200000);

	distanceQ.add(treePosList[0]+1, treePosList[0]);
	countQ.add(treePosList[0] + 1, 1);

	for (int index = 1; index < N; index++) {
		int treePos = treePosList[index];
		long long diff = 0;

		long long leftCount = countQ.sum(treePos+1);
		long long leftSum = distanceQ.sum(treePos + 1);
		diff += abs(treePos*leftCount - leftSum);
		diff += abs(treePos*(index - leftCount) - (distanceQ.sum(200000) - leftSum));
		diff %= MOD;

		ret *= diff;
		ret %= MOD;

		distanceQ.add(treePos+1, (long long)treePos);
		countQ.add(treePos+1, 1);
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
		auto ret = plantTreeCost();
#ifdef _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}