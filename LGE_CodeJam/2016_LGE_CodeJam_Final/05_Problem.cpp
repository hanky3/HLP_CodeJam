#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
using namespace std;
#if 0
//########################
// Constant Variable
//########################
const int MAX_TREE_NUM = 101;
const int MAX_HEIGHT = 100001;
const int INF = 987654321;


//########################
// Global Variable
//########################
FILE *fpInput = NULL;
FILE *fpOutput = NULL;

struct TreeNode {
	int index;
	int height;
	vector<pair<int, int>> adjTrees;
};

// 실제 입력정보를 관리하기 위한 전역변수
int treeNodeCnt, adjTreeCnt;
int startHeight, endHeight;
TreeNode treeList[MAX_TREE_NUM];
bool treeVisited[MAX_TREE_NUM];

bool allTreeVisited()
{
	for (int index = 0; index < treeNodeCnt; index++)
	{
		if (!treeVisited[index])	return false;
	}
	return true;
}


int cache[MAX_TREE_NUM][MAX_HEIGHT][2];
int minTimeToEndTree(int depth, int currTree, int currHeight, bool bRocket)
{
	// 마지막 위치에 도착한 경우
	if (currTree == treeNodeCnt - 1) {
		return abs(currHeight - endHeight);
	}


	// 모든 나무에 방문한 경우
	if (allTreeVisited())
		return INF;

	// 이미 방문한 나무인 경우
	if (treeVisited[currTree])
		return INF;

//	int &ret = cache[currTree][currHeight][bRocket == true ? 1 : 0];
//	if (ret > -1)
//		return ret;

	int ret = INF;
	vector<pair<int, int>> adjTrees = treeList[currTree].adjTrees;
	pair<int, int> nextTree;
	int temp = INF;
	bool rockFlag = false;
	for (auto adjTreeInfo : adjTrees)
	{
		int minDistance = adjTreeInfo.second;
		int newTreeHeight = 0;

		// 로케트를 사용하지 않은 경우, 로켓트를 사용하는 경우에 대한 최소이동거리 계산.
		if (!bRocket)
		{
			newTreeHeight = currHeight;
			if (currHeight > treeList[adjTreeInfo.first].height) {
				newTreeHeight = treeList[adjTreeInfo.first].height;
				minDistance += (currHeight - newTreeHeight);
			}

			treeVisited[currTree] = true;
			ret = min(ret, minDistance + minTimeToEndTree(depth+1, adjTreeInfo.first, newTreeHeight, true));
			treeVisited[currTree] = false;

			if (temp > ret) {
				temp = ret;
				nextTree = make_pair(adjTreeInfo.first, minDistance);
				rockFlag = true;
			}
		}

		// 로켓 사용하지 않고, 이동가능한지를 체크한다.
		if (treeList[currTree].height <= adjTreeInfo.second)
			continue;

		// 최소 이동거리를 계산한다.
		minDistance = adjTreeInfo.second;
		int upperBound = (treeList[adjTreeInfo.first].height + adjTreeInfo.second);
		int lowerBound = (adjTreeInfo.second + 1);
		if (currHeight >= lowerBound && currHeight <= upperBound) {
			minDistance += 0;
			newTreeHeight = currHeight - adjTreeInfo.second;
		}
		else if (currHeight > upperBound) {
			minDistance += currHeight - upperBound;
			newTreeHeight = treeList[adjTreeInfo.first].height;
		}
		else
		{
			minDistance += lowerBound - currHeight;
			newTreeHeight = lowerBound - adjTreeInfo.second;
		}

		treeVisited[currTree] = true;
		ret = min(ret, minDistance + minTimeToEndTree(depth + 1, adjTreeInfo.first, newTreeHeight, bRocket));
		treeVisited[currTree] = false;

		if (temp > ret) {
			temp = ret;
			nextTree = make_pair(adjTreeInfo.first, minDistance);
			rockFlag = false;
		}
	}

	fprintf(fpOutput, "[TreeInfo]%d %d %d (%d, %d) : %d(%d)\n", depth, currTree, currHeight, nextTree.first, nextTree.second, ret, rockFlag);
	return ret;
}

void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");
	if (!isFile)
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else
	{
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d\n", &testcase);

	for (int i = 0; i < testcase; i++)
	{
		int coordsCnt = 0;
		fscanf(fpInput, "%d %d\n", &treeNodeCnt, &adjTreeCnt);
		memset(treeVisited, 0, sizeof(treeVisited));
		memset(cache, -1, sizeof(cache));

		for (auto index = 0; index < treeNodeCnt; index++)
		{
			fscanf(fpInput, "%d", &treeList[index].height);
			treeList[index].adjTrees.clear();
		}

		for (auto index = 0; index < adjTreeCnt; index++)
		{
			pair<int, int> treeEdge;
			int distance;
			fscanf(fpInput, "%d %d %d\n", &treeEdge.first, &treeEdge.second, &distance);
			treeList[treeEdge.first - 1].adjTrees.push_back(make_pair(treeEdge.second - 1, distance));
			treeList[treeEdge.second - 1].adjTrees.push_back(make_pair(treeEdge.first - 1, distance));
		}
		fscanf(fpInput, "%d %d\n", &startHeight, &endHeight);

		int ret = minTimeToEndTree(1, 0, startHeight, false);
		if (ret == INF)
			ret = -1;

		fprintf(fpOutput, "%d", ret);
		if (isFile)
			printf("%d", ret);


		fprintf(fpOutput, "\n");
		if (isFile)
			printf("\n");
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
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif