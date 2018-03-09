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


using namespace std;

#define MOD 1000000007
#define MAX_TREE	2000

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

struct TreeInfo
{
	int x;
	int bottomY;
	int topY;
};

int treeCount;
vector<TreeInfo> treeInfoList;
int cacheTreeCount[MAX_TREE][2];

void readInputData()
{
	fscanf(fpInput, "%d", &treeCount);
	treeInfoList = vector<TreeInfo>(treeCount);

	memset(cacheTreeCount, -1, sizeof(cacheTreeCount));

	for (int index = 0; index < treeCount; index++) {
		TreeInfo tree;
		fscanf(fpInput, "%d %d %d", &tree.x, &tree.bottomY, &tree.topY);
		treeInfoList[index] = tree;
	}

	sort(treeInfoList.begin(), treeInfoList.end(), [](TreeInfo &a, TreeInfo &b) {
		return (a.x < b.x);
	});
}

bool treeIntersected(int startTree, bool startBottom, int endTree, bool endBottom)
{
	pair<int, int> startPos;
	pair<int, int> endPos;

	if (endTree - startTree <= 1)
		return true;

	startPos.first = treeInfoList[startTree].x;
	startPos.second = startBottom ? treeInfoList[startTree].bottomY : treeInfoList[startTree].topY;

	endPos.first = treeInfoList[endTree].x;
	endPos.second = endBottom ? treeInfoList[endTree].bottomY : treeInfoList[endTree].topY;

	bool intersected = true;
	for (int tree = startTree + 1; tree < endTree; tree++)
	{
		// intersectionY
		int midX = treeInfoList[tree].x;
		double temp = (startPos.second + endPos.second) * (double)(midX - startPos.first) / (endPos.first - startPos.first);
		int midY = (int)(temp + 0.5);

		if (midY < treeInfoList[endTree].bottomY || midY > treeInfoList[endTree].topY) {
			intersected = false;
			break;
		}
	}

	return intersected;
}

int treeMovingCount(int curr, bool isBottom)
{
	if (curr == (treeInfoList.size() - 1))
		return 1;

	int &ret = cacheTreeCount[curr][isBottom];

	if (ret >= 0)
		return ret;

	int start = curr;
	ret = 0;
	for (int next = start + 1; next < treeInfoList.size(); next++)
	{
		int count = 0;
		
		// check available(next bottom);
		if (treeIntersected(curr, isBottom, next, true)) 
		{
			count += treeMovingCount(next, true);
			count %= MOD;
		}
		// check available(next top)
		if (treeIntersected(curr, isBottom, next, false))
		{
			count += treeMovingCount(next, false);
			count %= MOD;
		}

		ret += count;
		ret %= MOD;
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
		int ret = treeMovingCount(0, true);
		ret += treeMovingCount(0, false);
		ret %= MOD;

		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);
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
