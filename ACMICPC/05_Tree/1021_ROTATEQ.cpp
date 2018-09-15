#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
#define MAX_NUM_VALUE	10000

FILE *fpInput;
FILE *fpOutput;

int N, M;
vector<int> numbers;
vector<int> erasePositions;

struct FenwickTree {
	vector<int> trees;
	FenwickTree(int n) : trees(n + 1, 0) {};
	void add(int pos, int val) {
		while (pos < trees.size()) {
			trees[pos] += val;
			pos += (pos & -pos);
		}
	}
	int sum(int pos) {
		int ret = 0;
		while (pos > 0) {
			ret += trees[pos];
			pos &= (pos - 1);
		}
		return ret;
	}
};

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);
	numbers.clear();
	for (int n = 1; n <= N; n++)
		numbers.push_back(n);

	erasePositions = vector<int>(M);
	for (int i = 0; i < M; i++) {
		fscanf(fpInput, "%d", &erasePositions[i]);
	}
}

int minNumberMove()
{
	int ret = 0;
	int currPos = 0;
	FenwickTree erasedQ(N);

	for (auto nextPos : erasePositions) {
		int erased = nextPos;
		nextPos -= erasedQ.sum(nextPos - 1);
		nextPos--;

		int moves = abs(currPos - nextPos);
		moves = min(moves, (int)numbers.size() - moves);
		ret += moves;

		auto itr = numbers.erase(numbers.begin() + nextPos);
		if (itr == numbers.end())
			itr = numbers.begin();
		currPos = itr - numbers.begin();
		erasedQ.add(erased, 1);
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
		auto ret = minNumberMove();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}