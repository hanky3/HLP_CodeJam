#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_DIGIT	5

int N;
vector<int> numbers;
vector<pair<int, int>> ranges;
void readInputData()
{
	fscanf(fpInput, "%d", &N);

	numbers = vector<int>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &numbers[index]);
	}

	ranges = vector<pair<int, int>>(N);
	for (int index = 0; index < N; index++) {
		ranges[index].first = max(0, index - numbers[index] + 1);
		ranges[index].second = min(N-1, index + numbers[index] - 1);
	}

	sort(ranges.begin(), ranges.end(), [&](pair<int, int> &a, pair<int, int> &b) {
		if (a.first < b.first) return true;
		if (a.first == b.first)
			return abs(a.first - a.second) > abs(b.first - b.second);
		return false;
	});
}

int minAnimalsToFire()
{
	int ret = 0;

	stack<pair<int, int>> st;

	for (auto r : ranges) {
		if (st.empty()) {
			st.push(r);
			continue;
		}

		pair<int, int> currOverlap = st.top();
		if (r.second <= currOverlap.second)
			continue;

		// erase
		pair<int, int> prev(-1, -1);
		while (!st.empty()) {
			currOverlap = st.top();
			if (currOverlap.second < (r.first - 1))
				break;
			st.pop();
			prev = currOverlap;
		}
		st.push(prev);
		st.push(r);
	}

	return st.size();
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
		int ret = minAnimalsToFire();
		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);

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