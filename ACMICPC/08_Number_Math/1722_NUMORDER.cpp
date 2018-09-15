#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
typedef unsigned long long uint64;

FILE *fpInput;
FILE *fpOutput;

int N;
int subProblem;
uint64 k;
vector<int> numbers;


void readInputData()
{
	fscanf(fpInput, "%d", &N);
	fscanf(fpInput, "%d", &subProblem);
	if (subProblem == 1)
		fscanf(fpInput, "%llu", &k);
	else {
		numbers = vector<int>(N);
		for (int index = 0; index < N; index++)
			fscanf(fpInput, "%d", &numbers[index]);
	}
}

uint64 factorial(int n)
{
	if (n == 0 || n == 1)	return 1;
	return n * factorial(n - 1);
}

void calcNumbers(int problem)
{
	vector<int> num(N, 0);
	for (int pos = 0; pos < N; pos++)
		num[pos] = (pos + 1);

	if (problem == 1) {
		vector<int> output;
		k--;
		for (int pos = 0; pos < N; pos++) {
			uint64 factorialN = factorial(N - pos - 1);
			int order = (int)(k / factorialN);
			output.push_back(num[order]);
			num.erase(num.begin()+order);
			k -= (order*factorialN);
		}
	
		for (auto n : output) {
#ifdef _FILE_
			printf("%d ", n);
#endif
			fprintf(fpOutput, "%d ", n);
		}
	}
	else {
		uint64 order = 1;

		int digit = N - 1;
		for (auto n : numbers) {
			auto itr = find(num.begin(), num.end(), n);
			int pos = itr - num.begin();
			order += (pos * factorial(digit));
			num.erase(itr);
			digit--;
		}
#ifdef _FILE_
		printf("%llu\n", order);
#endif
		fprintf(fpOutput, "%llu\n", order);

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
		calcNumbers(subProblem);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
