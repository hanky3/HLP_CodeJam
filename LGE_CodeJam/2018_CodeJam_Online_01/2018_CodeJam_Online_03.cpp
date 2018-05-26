#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>


using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N;
vector<int> numbers;

void readInputData()
{
	fscanf(fpInput, "%d\n", &N);
	numbers = vector<int>(N, 0);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &numbers[index]);
	}
	sort(numbers.begin(), numbers.end());
}

int minDiceTopCount()
{
	int count = 0;
	while (!numbers.empty()) {
		int diceCount = 0;

		auto itr = numbers.begin();
		while (itr != numbers.end()) {
			if (numbers.back() < diceCount)
				break;

			int num = *itr;
			if (num >= diceCount) {
				itr = numbers.erase(itr);
				diceCount++;
				continue;
			}
			itr++;
		}

		count++;
	}
	return count;
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
		int ret = minDiceTopCount();
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
