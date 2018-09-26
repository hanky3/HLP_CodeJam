#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;
#define MAX_SIZE			151
typedef unsigned long long uint64;

uint64 A, B, C;

void readInputData()
{
	fscanf(fpInput, "%llu %llu %llu", &A, &B, &C);
}

uint64 multiply(uint64 a, uint64 b, uint64 mod)
{
	uint64 multi = 0;
	uint64 half = 0;

	if (b == 1) {
		return (a % mod);
	}

	half = multiply(a, (b / 2), mod);
	multi = (half * half) % mod;
	if (b % 2) {
		multi *= a;
		multi %= mod;
	}
	return multi;
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
	while (testCase-- > 0) {
		readInputData();
		auto ret = multiply(A, B, C);
#ifdef _FILE_
		printf("%llu\n", ret);
#endif
		fprintf(fpOutput, "%llu\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
