#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_NUMBER	100
#define MAX_PLAYER	201

FILE *fpInput;
FILE *fpOutput;

int N, X, M, T;
vector<int> observeExpects;
vector<int> interests;
double cache[2][MAX_PLAYER];

void readInputData()
{
	fscanf(fpInput, "%d %d %d %d", &N, &X, &M, &T);
	memset(cache, -1, sizeof(cache));

	observeExpects = vector<int>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &observeExpects[index]);
		observeExpects[index]--;

	}

	interests = vector<int>(M);
	for (int index = 0; index < M; index++) {
		fscanf(fpInput, "%d", &interests[index]);
		interests[index]--;
	}
}

vector<double> createMatchingProb(int moves)
{
	memset(cache, 0, sizeof(cache));

	cache[0][0] = 1;

	int move = 1;
	while (move <= moves) {
		for (int n = 0; n < N; n++)
			cache[move % 2][n] = 0;

		for (int curr = 0; curr < N; curr++) {
			int next = observeExpects[curr];
			if (curr == 0) {
				cache[move % 2][next] += cache[(move - 1) % 2][curr];
				continue;
			}

			for (int nextCand = next - T; nextCand <= next + T; nextCand++) {
				int pos = (nextCand + N) % N;
				cache[move % 2][pos] += (cache[(move - 1) % 2][curr] / (double)(2 * T + 1));
			}

		}

		bool unchanged = true;
		for (int n = 0; n < N; n++) {
			if (abs(cache[0][n] - cache[1][n]) > 1e-7) {
				unchanged = false;
				break;
			}
		}

		if (unchanged) break;

		move++;
	}
	if (move > X)
		move = X;

	vector<double> ret(cache[move % 2], cache[move % 2] + N);
	return ret;
}

void generateOutput(bool isFile)
{
	vector<double> prob = createMatchingProb(X);
	for (auto p : interests) {
		double ret = prob[p];
		if (isFile)
			printf("%.5f ", ret);
		fprintf(fpOutput, "%.5f ", ret);
	}
	if (isFile)
		printf("\n");
	fprintf(fpOutput, "\n");

}

void solveProblem(const char *fileName, bool isFile)
{
	fpInput = stdin;
	fpOutput = stdout;
	if (isFile) {
		fpInput = fopen(fileName, "r");
		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		generateOutput(isFile);

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
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