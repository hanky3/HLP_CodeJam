#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <tuple>

using namespace std;
typedef long long int64;

FILE *fpInput;
FILE *fpOutput;

int N, M, K;
vector<int64> numbers;
vector<tuple<int, int, int>> querys;

struct FenwickTree {
	vector<int64> trees;
	FenwickTree(int64 n) : trees(n + 1, 0ll) {};
	void add(int pos, int64 val) {
		while (pos < trees.size()) {
			trees[pos] += val;
			pos += (pos & -pos);
		}
	}
	int64 sum(int pos) {
		int64 ret = 0;
		while (pos > 0) {
			ret += trees[pos];
			pos &= (pos - 1);
		}
		return ret;
	}
};

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &N, &M, &K);

	numbers = vector<int64>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%lld", &numbers[i]);
	}
	querys.clear();

	for (int i = 0; i < M + K; i++) {
		int a, b, c;
		fscanf(fpInput, "%d %d %d", &a, &b, &c);
		querys.push_back(make_tuple( a, b, c ));
	}
}

vector<int64> calcRangeSums()
{
	vector<int64> ret;
	int currPos = 0;
	FenwickTree sumQ(N);
	for (int pos = 1; pos <= N; pos++) {
		sumQ.add(pos, numbers[pos-1]);
	}

	for (auto q : querys) {
		int cmd = std::get<0>(q);
		int a = std::get<1>(q);
		int b = std::get<2>(q);

		if (cmd == 1) {
			sumQ.add(a, -numbers[a - 1]);
			sumQ.add(a, b);
			numbers[a - 1] = b;
		}
		else {
			int64 psum = sumQ.sum(b) - sumQ.sum(a - 1);
			ret.push_back(psum);
		}
	}
	return ret;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	string outputFileName = string(argv[1]);
	fpOutput = fopen("output.txt", "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = calcRangeSums();
		for (auto n : ret) {
#ifdef _FILE_
			printf("%lld\n", n);
#endif
			fprintf(fpOutput, "%lld\n", n);
		}
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

