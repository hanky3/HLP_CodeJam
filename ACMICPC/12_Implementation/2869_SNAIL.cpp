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

#define INF		987654321
#define MAX_V	811

int A, B, V;

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &A, &B, &V);
}

int snailRiseDays()
{
	int days = 0;
	int diff = A - B;

	days = ((V-A) / diff);
	int remains = V - (days * diff);
	while (remains > 0) {
		days++;
		if (remains - A <= 0)
			break;
		remains -= diff;
	}

	return days;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#if _FILE_
	fpInput = fopen(argv[1], "r");
	fpOutput = fopen("output.txt", "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		auto ret = snailRiseDays();
#if _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
