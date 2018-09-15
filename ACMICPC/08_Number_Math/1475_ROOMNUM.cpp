#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
}

int minNumberSets(int num)
{
	int ret = 0;
	vector<int> numCount(10, 0);

	if (num == 0)		return 1;

	while (num > 0) {
		int curr = num % 10;
		if (curr == 9)	curr = 6;
		numCount[curr]++;
		num /= 10;
	}
	numCount[6] = (numCount[6]/2) + (numCount[6]%2);
	ret = *max_element(numCount.begin(), numCount.end());
	return ret;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	string outputFileName = string(argv[1]);
	outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
	fpOutput = fopen(outputFileName.c_str(), "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = minNumberSets(N);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
