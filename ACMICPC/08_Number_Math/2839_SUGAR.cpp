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

int N;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
}


int minDeliveryBags()
{
	int ret = 0;
	for (int fiveTimes = (N / 5); fiveTimes >= 0; fiveTimes--) {
		int remains = N - fiveTimes * 5;
		if (remains % 3 == 0) {
			return (fiveTimes + (remains / 3));
		}
	}
	return -1;
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
		auto ret = minDeliveryBags();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
