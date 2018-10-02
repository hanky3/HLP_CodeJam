#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>


using namespace std;
typedef long long int64;
FILE *fpInput;
FILE *fpOutput;

#define INF		987654321
#define MAX_HEIGHT 500001
#define MAX_N				51

int N;
vector<int> heights;
string password;
int cache[MAX_N][MAX_HEIGHT];

void readInputData()
{
	memset(cache, -2, sizeof(cache));
	fscanf(fpInput, "%d", &N);
	heights = vector<int>(N);
	for (int i = 0; i < N; i++)
		fscanf(fpInput, "%d", &heights[i]);
}

int maxTopHeight(int step, int diff)
{
	int &ret = cache[step][diff];
	if (ret > -2)	return ret;
	if (step >= N) {
		if (diff == 0) return 0;
		return -1;
	}

	int currHeight = heights[step];

	// single code
	ret = -1;
	int next;

	next = maxTopHeight(step + 1, diff);
	if (next > -1) ret = max(ret, next);
	next = maxTopHeight(step + 1, abs(currHeight - diff));
	if (next > -1) ret = max(ret, currHeight+next);
	next = maxTopHeight(step + 1, currHeight + diff);
	if (next > -1) ret = max(ret, currHeight+next);

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
		auto ret = maxTopHeight(0, 0);
		if (ret > 0) ret /= 2;
		else ret = -1;

#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

