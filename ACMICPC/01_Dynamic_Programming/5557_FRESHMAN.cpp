#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
FILE *fpInput;
FILE *fpOutput;
#define MAX_STEP		101
#define MAX_SUM		20
typedef long long int64;

int N;
vector<int> numbers;
int64 cache[MAX_STEP][MAX_SUM+1][2];

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	memset(cache, -1, sizeof(cache));

	numbers = vector<int>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &numbers[index]);
	}
}

int64 possibleFormula(int step, int sum, bool minus)
{
	int64 &ret = cache[step][sum][minus];
	if (ret != -1)
		return ret;

	if (step == N - 1) {
		return (sum == numbers[step]) ? 1 : 0;
	}

	if (minus)
		sum -= numbers[step];
	else
		sum += numbers[step];

	ret = 0;
	if (sum >= 0 && sum <= MAX_SUM) {
		ret += possibleFormula(step + 1, sum, true);
		if (step < N-2)
			ret += possibleFormula(step + 1, sum, false);
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
		auto ret = possibleFormula(0, 0, 0);
#ifdef _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
