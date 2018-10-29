#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_LIS_SIZE		510

int numberCnt;
unsigned long long skip;
vector<vector<int>> lisStart;
vector<double> lisCount;
vector<vector<int>> lisResult;

vector<int> numbers;

int cache[MAX_LIS_SIZE];

void readInputData()
{
	fscanf(fpInput, "%d %llu", &numberCnt, &skip);
	memset(cache, -1, sizeof(cache));

	lisStart = vector<vector<int>>(numberCnt + 1, vector<int>());
	lisCount = vector<double>(numberCnt + 1, numeric_limits<double>::max());
	numbers = vector<int>(numberCnt);
	lisResult.clear();

	memset(cache, -1, sizeof(cache));

	for (int index = 0; index < numberCnt; index++)
		fscanf(fpInput, "%d", &numbers[index]);
}

int maxLisLength(int start)
{
	int &ret = cache[start+1];
	if (ret != -1)
		return ret;

	if (start == (numbers.size() -1)) {
		ret = 1;
		return ret;
	}
	
	int curr = 0;
	if (start >= 0)
		curr = numbers[start];

	ret = 1;
	vector<int> lisNext;
	for (int next = start + 1; next < numberCnt; next++) {
		if (numbers[next] < curr)
			continue;
	
		int lisLength = 1 + maxLisLength(next);
		if (start < 0)
			lisLength--;
		if (ret < lisLength) {
			lisNext.clear();
			lisNext.push_back(next+1);
		}
		else if (ret == lisLength) 
			lisNext.push_back(next+1);

		ret = max(ret, lisLength);
	}
	sort(lisNext.begin(), lisNext.end(), [](int &a, int &b) {
		return (numbers[a - 1] < numbers[b - 1]);
	});
	lisStart[start+1] = lisNext;

	return ret;
}

double calcLisCount(int start)
{
	vector<int> lisNext = lisStart[start + 1];
	double &ret = lisCount[start + 1];

	if (ret != numeric_limits<double>::max())
		return ret;

	if (lisStart[start + 1].empty()) {
		ret = 1;
		return ret;
	}

	ret = 0;
	for (auto next : lisStart[start + 1]) {
		ret += calcLisCount(next - 1);
	}

	return ret;
}


bool reconstruct(vector<int> &selected, int start)
{
	vector<int> lisNext = lisStart[start+1];

	if (lisStart[start + 1].empty()) {
		//lisResult.push_back(selected);
		skip--;
		if (skip == 0) {
			lisResult.push_back(selected);
			return true;
		}
		return false;
	}

	for (auto next : lisStart[start+1]) {
		double totalLisCount = lisCount[next];

		if (totalLisCount < skip) {
			skip -= (int)totalLisCount;
			continue;
		}

		selected.push_back(numbers[next-1]);
		if (reconstruct(selected, next-1))
			return true;
		selected.pop_back();
	}

	return false;
}


void kthLis()
{
	vector<int> selected;
	calcLisCount(-1);
	reconstruct(selected, -1);
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
		int ret = maxLisLength(-1);
		kthLis();
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);

		if (isFile) {
			for (auto n : lisResult[0])
				printf("%d ", n);
			printf("\n");
		}
		for (auto n : lisResult[0])
			fprintf(fpOutput, "%d ", n);
		fprintf(fpOutput, "\n");

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}