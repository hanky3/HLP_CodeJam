#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
using namespace std;

//########################
// Constant Variable
//########################
const int MAX_TREE_NUM = 102;
const int MAX_HEIGHT = 100002;
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
vector<bool> treeVisited;
bool rocketUsed;
vector<int> currTreeHeight;

// 다익스트라 알고리즘을 변형하여 시작과 끝의 최단거리를 계산한다.
vector<int> findShortestPath(int start, int end, int initialHeight, int rocketTree)
{
	vector<int> treeDistanceList = vector<int>(treeNodeCnt, INF);
	priority_queue<pair<int, pair<int, int>>> pq;
	treeVisited.clear();
	treeVisited = vector<bool>(treeNodeCnt, false);
	currTreeHeight.clear();
	currTreeHeight = vector<int>(treeNodeCnt);

	pq.push(make_pair(0, make_pair(start, initialHeight)));
	treeDistanceList[start] = 0;
	currTreeHeight[start] = initialHeight;
	while (!pq.empty())
	{
		int cost = -pq.top().first;
		pair<int, int> here = pq.top().second;

		pq.pop();
		if (treeDistanceList[here.first] < cost) continue;
		//if (treeVisited[here.first]) continue;

		if (here.first == end)
			return treeDistanceList;

		// 갈수 있는 정점중에 가장 가까운 정점 선택
		vector<pair<int, int>> adjTrees = treeList[here.first].adjTrees;
		for (auto adjTreeInfo : adjTrees)
		{
			int there = adjTreeInfo.first;
			int thereDist = adjTreeInfo.second;

			int nextDist = cost + thereDist;
			int nextHeight = 0;

			if (here.first == rocketTree)
			{
				nextHeight = here.second;
				if (here.second > treeList[there].height)
				{
					nextHeight = treeList[there].height;
					nextDist += (here.second - treeList[there].height);
				}
			}
			else
			{
				// 로켓 사용하지 않고, 이동가능한지를 체크한다.
				if (treeList[here.first].height <= thereDist)
					continue;

				// 최소 이동거리를 계산한다.
				int upperBound = (treeList[adjTreeInfo.first].height + adjTreeInfo.second);
				int lowerBound = (adjTreeInfo.second + 1);
				if (here.second >= lowerBound && here.second <= upperBound) {
					nextHeight = here.second - adjTreeInfo.second;
				}
				else if (here.second > upperBound) {
					nextDist += here.second - upperBound;
					nextHeight = treeList[adjTreeInfo.first].height;
				}
				else {
					nextDist += lowerBound - here.second;
					nextHeight = 1;
				}
			}

			// 마지막 트리인 경우 distance 갱신필요
			if (there == (treeNodeCnt - 1))
				nextDist += abs(nextHeight - endHeight);


			if (treeDistanceList[there] > nextDist)
			{
				treeDistanceList[there] = nextDist;
				pq.push(make_pair(-nextDist, make_pair(there, nextHeight)));

				currTreeHeight[there] = nextHeight;
			}
			else if (treeDistanceList[there] == nextDist)
			{
				//currTreeHeight[there].push_back(nextHeight);
			}
		}
	}
	return treeDistanceList;
}

int findShortestPathWithRocket(int start, int initialHeight)
{
	int ret = INF;
	for (int treeIndex = 5; treeIndex < treeNodeCnt; treeIndex++)
	{
		int newDistance = 0;
		vector<int> treeDistance = findShortestPath(start, treeIndex, initialHeight, treeIndex);
		newDistance += treeDistance[treeIndex];

		if (newDistance >= INF) continue;

		if (treeIndex < treeNodeCnt - 1) {
			treeDistance = findShortestPath(treeIndex, treeNodeCnt - 1, currTreeHeight[treeIndex], treeIndex);
			newDistance += treeDistance[treeNodeCnt - 1];
		}

		ret = min(ret, newDistance);
	}

	if (ret == INF)
		ret = -1;
	//	if (ret != INF)
	//		ret += abs(startHeight - currTreeHeight[0]);

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
		//memset(treeVisited, 0, sizeof(treeVisited));
		//memset(cache, -1, sizeof(cache));

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

		int ret = findShortestPathWithRocket(0, startHeight);

		/*
		vector<int> treeDistance = findShortestPath(treeNodeCnt - 1, endHeight);
		int ret = treeDistance[0];

		//int ret = minTimeToEndTree(0, startHeight, true);
		if (ret == INF)
		ret = -1;
		else
		ret += abs(startHeight - currTreeHeight[0]);
		*/

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
	solveProblem(argv[1], true);
	return 0;
}
