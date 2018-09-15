#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

long long A, B;

void readInputData()
{
	fscanf(fpInput, "%lld %lld", &A, &B);
	if (A > B)
		swap(A, B);
}

long long totalSum(long long start, long long end)
{
	long long ret = (end - start + 1) * (end + start) / 2;
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
		auto ret = totalSum(A, B);
#ifdef _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
