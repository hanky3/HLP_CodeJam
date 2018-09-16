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
vector<int> positions;

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

	positions = vector<int>(M);
	for (int i = 0; i < M; i++) {
		fscanf(fpInput, "%d", &positions[i]);
		positions[i]--;
	}
}

void moviePositions(int n, int m)
{
	int qSize = n + m + 1;
	FenwickTree erasedQ(n+m+1);

	vector<int> moviePos(n, 0);
	for (int p = 0; p < n; p++) {
		moviePos[p] = n - p;
	}

	int index = 0;
	int height = n+1;
	for (auto p : positions) {
		int cnt = moviePos[p] - erasedQ.sum(moviePos[p]);
		erasedQ.add(moviePos[p], 1);
		moviePos[p] = height++;

#ifdef _FILE_
		printf("%d ", n - cnt);
#endif
		fprintf(fpOutput, "%d ", n - cnt);
	}
#ifdef _FILE_
	printf("\n");
#endif
	fprintf(fpOutput, "\n");

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
	fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		moviePositions(N, M);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}