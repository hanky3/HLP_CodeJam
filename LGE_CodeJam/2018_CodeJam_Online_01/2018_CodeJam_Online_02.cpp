#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_WORD_SIZE		101

int N, K, M;
vector<int> numbers;
vector<int> reverseInfo;
void readInputData()
{
	fscanf(fpInput, "%d %d %d\n", &N, &K, &M);
	numbers = vector<int>(N, 0);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &numbers[index]);
	}
	reverseInfo = vector<int>(M, 0);
	for (int index = 0; index < M; index++) {
		fscanf(fpInput, "%d", &reverseInfo[index]);
	}
}

int finalNumPosition(int pos)
{
	int currPos = pos - 1;
	for (auto q : reverseInfo) {
		int start = 0;
		if (q < 0)	start = numbers.size() - abs(q);
		int end = start + abs(q) - 1;
		// range check
		if (currPos < start || currPos > end)
			continue;

		currPos = end - (currPos - start);
	}

	return currPos+1;
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
	while (testCase > 0)
	{
		readInputData();
		int ret = finalNumPosition(K);
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
