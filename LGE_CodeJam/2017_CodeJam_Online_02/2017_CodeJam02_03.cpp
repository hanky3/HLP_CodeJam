#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <time.h>

#if 0
using namespace std;
#define MAX_WEIGHT 1000000001

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
int L, M, N;
vector<pair<int, int>> colorVertexs;

enum V_COLOR {
	V_RED = 0, V_BLUE, V_PURPLE
};

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &L, &M, &N);
	colorVertexs = vector<pair<int, int>>(L + M + N, make_pair(0, 0));
	
	int colorIndex = V_RED;
	for (int index = 0; index < colorVertexs.size(); index++) {
		if (index >= L && index < (L + M))
			colorIndex = V_BLUE;
		else if (index >= (L + M))
			colorIndex = V_PURPLE;

		fscanf(fpInput, "%d", &colorVertexs[index].first);
		colorVertexs[index].second = colorIndex;

	}
	sort(colorVertexs.begin(), colorVertexs.end());
}

unsigned long long minWeightGraph()
{
	unsigned long long ret = 0;

	int prevPurplePos = -1;
	int prevRedPos = -1;
	int prevBluePos = -1;
	int maxRWeight = 0, maxBWeight = 0;

	for (int index = 0; index < colorVertexs.size(); index++) {
		int color = colorVertexs[index].second;
		int pos = colorVertexs[index].first;

		if (color == V_RED || color == V_PURPLE) {
			if (prevRedPos >= 0) {
				ret += (pos - prevRedPos);
				maxRWeight = max(maxRWeight, (pos - prevRedPos));
			}
			prevRedPos = pos;
		}

		if (color == V_BLUE || color == V_PURPLE) {
			if (prevBluePos >= 0) {
				ret += (pos - prevBluePos);
				maxBWeight = max(maxBWeight, (pos - prevBluePos));
			}
			prevBluePos = pos;
		}

		if (color == V_PURPLE) {
			// cycle check
			if (prevPurplePos >= 0) {
				int dist = (pos - prevPurplePos);

				if ((maxBWeight + maxRWeight) > dist) {
					ret -= maxBWeight;
					ret -= maxRWeight;
					ret += dist;
				}
			}
			maxBWeight = maxRWeight = 0;
			prevPurplePos = pos;
		}
	}

	return ret;
}

void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");

	if (!isFile)	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else {
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++) {
		// read
		readInputData();

		// output
		unsigned long long ret = minWeightGraph();
		fprintf(fpOutput, "%llu\n", ret);
		if (isFile)
			printf("%llu\n", ret);
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if 1
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif