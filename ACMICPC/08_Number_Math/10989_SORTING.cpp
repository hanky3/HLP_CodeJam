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

int N;
int numbers[MAX_NUM_VALUE + 1];

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	memset(numbers, 0, sizeof(numbers));
	for (int i = 0; i < N; i++) {
		int num;
		fscanf(fpInput, "%d", &num);
		numbers[num]++;
	}
}

void printSortingNum()
{
	int num = 0;
	for (auto len : numbers) {
		while (len-- > 0) {
#ifdef _FILE_
			printf("%d\n", num);
#endif
			fprintf(fpOutput, "%d\n", num);
		}
		num++;
	}
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
		printSortingNum();
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
