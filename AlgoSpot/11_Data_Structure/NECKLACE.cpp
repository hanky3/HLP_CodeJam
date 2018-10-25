#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;
typedef unsigned long long uint64;

FILE *fpInput;
FILE *fpOutput;

int N, C;
vector<uint64> beadCount;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &C);

	beadCount = vector<uint64>(C, 0);
	for (int index = 0; index < C; index++) {
		fscanf(fpInput, "%llu", &beadCount[index]);
	}

	sort(beadCount.begin(), beadCount.end());
}

uint64 maxNecklace()
{
	if (beadCount.size() < N)	return 0;

	while (beadCount.size() > N) {
		uint64 increase = beadCount[0];

		for (int step = 0; step < N; step++) {
			int curr = beadCount.size() - N + step;
			if (increase == 0) break;

			uint64 maxDivide = numeric_limits<uint64>::max();
			if (step < (N - 1)) {
				maxDivide = beadCount[curr + 1] - beadCount[curr];
			}
			if (maxDivide == 0)
				continue;

			int divide = increase / (step + 1);
			int remains = increase % (step + 1);
			divide = min<uint64>(maxDivide, divide);
			if (divide >= maxDivide)
				remains = 0;
			for (int prev = 0; prev <= step; prev++) {
				uint64 &neckNum = beadCount[beadCount.size() - N + prev];
				neckNum += divide;
				if ((step - prev) <= remains-1)
					neckNum += 1;
			}
			increase -= (divide * (step + 1));
			increase -= remains;

		}
		beadCount.erase(beadCount.begin());
	}
	return beadCount[0];
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
		auto ret = maxNecklace();
		if (isFile)
			printf("%llu\n", ret);
		fprintf(fpOutput, "%llu\n", ret);
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