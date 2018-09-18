#include <stdio.h>
#include <string.h>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;
typedef long long int64;

FILE *fpInput;
FILE *fpOutput;


int N;
vector<int> heights;

void readInputData()
{
	fscanf(fpInput, "%d\n", &N);
	heights = vector<int>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%d", &heights[i]);
	}
}

int64 calcRectArea(int start, int end)
{
	int64 area;
	int left = (start + end) / 2;
	int right = left+1;

	int64 maxHeight = min(heights[left], heights[right]);
	area = (right - left + 1) * maxHeight;
	while (left >= start || right <= end) {
		
		if (left >= start && right <= end)
			area = max(area, (int64)maxHeight *(right - left + 1));

		int64 nextHeight[2] = { -1, -1 };
		if (left > start)	nextHeight[0] = heights[left - 1];
		if (right < end)	nextHeight[1] = heights[right+1];

		bool goLeft = false;
		if (left >= start && nextHeight[0] >= nextHeight[1]) {
			goLeft = true;
		}

		if (goLeft) {
			left--;
			maxHeight = min(maxHeight, nextHeight[0]);
		}
		else {
			right++;
			maxHeight = min(maxHeight, nextHeight[1]);
		}
	}
	return area;
}

int64 maxRectangle(int start, int end)
{
	int64 area = 0;
	if (start == end)
		return heights[start];

	int mid = (start + end) / 2;
	int64 left = maxRectangle(start, mid);
	int64 right = maxRectangle(mid+1, end);
	area = max(left, right);
	area = max(area, calcRectArea(start, end));

	return area;
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
	while (true) {
		readInputData();
		if (N == 0)
			break;

		auto ret = maxRectangle(0, N-1);
#ifdef _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}


