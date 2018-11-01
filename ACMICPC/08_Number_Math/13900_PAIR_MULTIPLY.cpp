#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>


using namespace std;
typedef long long int64;

FILE *fpInput;
FILE *fpOutput;

int N;
vector<int> numbers;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	numbers = vector<int>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%d", &numbers[i]);
	}
	sort(numbers.begin(), numbers.end());
}

int64 sumPairMultiply()
{
	int64 ret = 0;
	int64 psum = 0;

	for (auto n : numbers) {
		ret += (n * psum);
		psum += n;
	}

	return ret;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	fpOutput = fopen("output.txt", "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = sumPairMultiply();
#if _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

