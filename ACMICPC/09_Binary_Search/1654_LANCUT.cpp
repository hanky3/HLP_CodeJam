#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;
typedef long long int64;

int N, K;
vector<int64> lineLength;

void readInputData()
{
	fscanf(fpInput, "%d %d", &K, &N);

	lineLength = vector<int64>(K);
	for (int index = 0; index < K; index++) {
		fscanf(fpInput, "%lld", &lineLength[index]);
	}
}

bool canGenerateLines(int64 length, int maxCount)
{
	int lineCount = 0;
	for (auto line : lineLength) {
		lineCount += (line / length);
	}
	return (lineCount >= maxCount);
}


int64 maxLineLength()
{
	int64 lo, hi, mid;
	lo = 0;
	hi = numeric_limits<int64>::max();

	while (lo + 1 < hi) {
		mid = (lo + hi) / 2;
		if (canGenerateLines(mid, N))
			lo = mid;
		else
			hi = mid;
	}

	return lo;
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
		auto ret = maxLineLength();
#ifdef _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
