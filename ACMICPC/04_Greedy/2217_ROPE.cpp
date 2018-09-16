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

int N;
vector<int> weights;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	weights = vector<int>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%d", &weights[i]);
	}
	sort(weights.begin(), weights.end(), [&](int a, int b) {
		return (a > b);
	});
}

int calcMaxWeight()
{
	int ret = 0;

	int ropeCnt = 0;
	for (auto w : weights) {
		ropeCnt++;
		ret = max(ret, w*ropeCnt);
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
	while (testCase > 0)	{
		readInputData();
		auto ret = calcMaxWeight();
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
