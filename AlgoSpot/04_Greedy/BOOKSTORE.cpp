#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <queue>
#include <assert.h>


using namespace std;

FILE *fpInput;
FILE *fpOutput;

typedef unsigned long long uint64;

int N, M;
vector<vector<pair<int, int>>> books;

void readInputData()
{
	fscanf(fpInput, "%d %d\n", &N, &M);
	books = vector<vector<pair<int, int>>>(M, vector<pair<int, int>>(N));

	for (int book = 0; book < N; book++) {
		for (int store = 0; store < M; store++) {
			pair<int, int> bookInfo;
			fscanf(fpInput, "%d %d", &bookInfo.first, &bookInfo.second);
			books[store][book] = bookInfo;
		}
	}

	for (int store = 0; store < M; store++) {
		vector<pair<int, int>> &bookCosts = books[store];
		sort(bookCosts.begin(), bookCosts.end(), [](pair<int, int> &a, pair<int, int> &b) {
			return (a.second > b.second);
		});
	}
}

int minimumBookCost()
{
	int ret = numeric_limits<int>::max();

	for (int store = 0; store < M; store++) {
		int point = 0;
		int totalCost = 0;
		for (int book = 0; book < N; book++) {
			int bookCost = books[store][book].first;
			bookCost -= point;
			point = 0;

			if (bookCost < 0) {
				point -= bookCost;
				bookCost = 0;
			}

			totalCost += bookCost;
			point += books[store][book].second;
		}
		ret = min(ret, totalCost);
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

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		int ret = minimumBookCost();
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);

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