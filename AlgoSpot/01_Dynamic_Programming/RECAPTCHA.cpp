#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define ALPHABET_SIZE				26
#define MAX_BOUNDARY_CNT		50

FILE *fpInput;
FILE *fpOutput;

int N;
vector<double> similarityValue[MAX_BOUNDARY_CNT][MAX_BOUNDARY_CNT];
double cache[MAX_BOUNDARY_CNT];
int nextBoundary[MAX_BOUNDARY_CNT];

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	memset(similarityValue, 0, sizeof(similarityValue));
	memset(cache, -1, sizeof(cache));
	memset(nextBoundary, -1, sizeof(nextBoundary));

	for (int start = 0; start < N; start++) {
		for (int end = 0; end < N; end++) {
			similarityValue[start][end] = vector <double>(ALPHABET_SIZE, 0.0);
		}
	}

	int similarityCnt = (N*(N - 1)) / 2;
	for (int start = 0; start < N; start++) {
		for (int end = start + 1; end < N; end++) {
			for (int alpha = 0; alpha < ALPHABET_SIZE; alpha++) {
				fscanf(fpInput, "%lf", &similarityValue[start][end][alpha]);
			}
		}
	}
}

void reconstruct(int start, string &ret)
{
	int next = nextBoundary[start];
	if (next == -1)
		return;

	vector<double> &simValue = similarityValue[start][next];
	int pos = max_element(simValue.begin(), simValue.end()) - simValue.begin();
	ret.push_back('a' + pos);
	reconstruct(next, ret);
}

double maxSimilarity(int start)
{
	if (start == (N-1))
		return 1.0f;

	double &ret = cache[start];
	if (ret > -1.0f)
		return ret;

	ret = 0.0f;
	for (int end = start + 1; end < N; end++) {
		vector<double> &simValues = similarityValue[start][end];
		double val = *max_element(simValues.begin(), simValues.end());

		double nextSimilarity = val * maxSimilarity(end);
		if (nextSimilarity > ret) {
			ret = nextSimilarity;
			nextBoundary[start] = end;
		}
	}
	return ret;
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
		string ret;

		readInputData();
		maxSimilarity(0);
		reconstruct(0, ret);
		if (isFile)
			printf("%s\n", ret.c_str());
		fprintf(fpOutput, "%s\n", ret.c_str());
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