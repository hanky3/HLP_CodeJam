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
int cache[2][MAX_HEIGHT];

void readInputData()
{
	memset(cache, -1, sizeof(cache));
	fscanf(fpInput, "%d", &N);
	heights = vector<int>(N);
	for (int i = 0; i < N; i++)
		fscanf(fpInput, "%d", &heights[i]);
}

int maxTopHeight()
{
	int ret = -1;
	cache[0][heights[0]] = heights[0];
	cache[0][0] = 0;
	int sumHeight = heights[0];
	int prev = 0;
	for (int step = 1; step < N; step++) {
		int curr = step % 2;
		for (int h = 0; h <= sumHeight; h++) {
			if (cache[prev][h] >= 0) {
				cache[curr][h] = max(cache[curr][h], cache[prev][h]);
				if (h + heights[step] < MAX_HEIGHT)
					cache[curr][heights[step] + h] = max(cache[curr][heights[step] + h], heights[step] + cache[prev][h]);
				if (h - heights[step] >= 0)
					cache[curr][h - heights[step]] = max(cache[curr][h - heights[step]], heights[step] + cache[prev][h]);
				if (heights[step] - h >= 0)
					cache[curr][heights[step] - h] = max(cache[curr][heights[step] - h], heights[step] + cache[prev][h]);
			}
		}
		prev = curr;
		sumHeight += heights[step];
	}

	if (cache[prev%2][0] > 0)
		ret = cache[prev % 2][0] / 2;
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
		auto ret = maxTopHeight();

#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

