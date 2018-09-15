#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

FILE *fpInput;
FILE *fpOutput;
int A, B;

void readInputData()
{
	fscanf(fpInput, "%d %d", &A, &B);
}

double divideNum(int a, int b)
{
	double ret;
	ret = (double)a / (double)b;
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
	while (testCase > 0) {
		readInputData();
		auto ret = divideNum(A, B);
#ifdef _FILE_
		printf("%.10f\n", ret);
#endif
		fprintf(fpOutput, "%.10f\n", ret);
		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);


	return 0;
}