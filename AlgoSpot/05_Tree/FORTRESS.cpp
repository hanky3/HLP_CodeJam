#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <list>
#include <functional>

using namespace std;
#define INF 0x0FFFFFFF
#define MAX_TREE_NODE 100

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

struct TreeNode {
	int nodeId;
	pair<int, int> pos;
	int radius;

	vector<int> parentNode;
	vector<int> childNodes;
};
int treeNodeCount;
vector<TreeNode> treeNodes;


bool findParent(int tree1, int tree2, int &parentTree)
{
	pair<int, int> tree1Pos = treeNodes[tree1].pos;
	pair<int, int> tree2Pos = treeNodes[tree2].pos;

	double distance = sqrt(pow(tree1Pos.first - tree2Pos.first, 2) + pow(tree1Pos.second - tree2Pos.second, 2));

	bool bRelated = false;
	if (treeNodes[tree1].radius >= distance || treeNodes[tree2].radius >= distance)
	{
		parentTree = treeNodes[tree1].radius > treeNodes[tree2].radius ? tree1 : tree2;
		bRelated = true;
	}

	return bRelated;
}

bool compare(int tree1, int tree2)
{
	return treeNodes[tree1].radius < treeNodes[tree2].radius;
}

int formTreeRelation()
{
	int rootTree = 0;
	for (int mainTree = 0; mainTree < treeNodeCount; mainTree++)
	{
		TreeNode treeNodeInfo = treeNodes[mainTree];
		for (int subTree = mainTree + 1; subTree < treeNodeCount; subTree++)
		{
			int parentTree;
			if (findParent(mainTree, subTree, parentTree))
			{
				if (parentTree == mainTree)
					treeNodes[subTree].parentNode.push_back(mainTree);
				else
					treeNodes[mainTree].parentNode.push_back(subTree);
			}
		}
		sort(treeNodes[mainTree].parentNode.begin(), treeNodes[mainTree].parentNode.end(), compare);

		if (mainTree != 0)
		{
			int parentTree = treeNodes[mainTree].parentNode[0];
			treeNodes[parentTree].childNodes.push_back(mainTree);
		}
	}
	return rootTree;
}

vector<int> treeDepthCache;
int treeMaxDepth(int treeNode)
{
	int &ret = treeDepthCache[treeNode];

	if (ret != -1)
		return ret;

	TreeNode treeNodeInfo = treeNodes[treeNode];
	if (treeNodeInfo.childNodes.size() == 0)
		return 0;

	ret = 0;
	for (int index = 0; index < treeNodeInfo.childNodes.size(); index++)
	{
		ret = max(ret, treeMaxDepth(treeNodeInfo.childNodes[index]) + 1);
	}
	return ret;
}


int maxTreeNodeDistance(int rootTree)
{
	int ret = 0;
	int cnt = 0;

	TreeNode treeNodeInfo = treeNodes[rootTree];
	if (treeNodeInfo.childNodes.size() == 0)
		return 0;

	vector<pair<int, int>> maxDepths;
	for (int index = 0; index < treeNodeInfo.childNodes.size(); index++)
	{
		int childTreeDepth = treeMaxDepth(treeNodeInfo.childNodes[index]) + 1;
		maxDepths.push_back(make_pair(childTreeDepth, treeNodeInfo.childNodes[index]));
	}
	sort(maxDepths.begin(), maxDepths.end(), std::greater<pair<int, int>>());
	vector<pair<int, int>>::iterator itr = maxDepths.begin();
	while (itr != maxDepths.end())
	{
		ret += itr->first;
		itr++; cnt++;
		if (cnt >= 2) break;
	}

	ret = max(ret, maxTreeNodeDistance(maxDepths[0].second));
	return ret;
}


// Solve the problem(read the input file / and retrieves the result
void solveProblem(char *filename, bool isFile)
{
	//	gFpOutput = fopen("Test.txt", "w");
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

	int testCase = 0;
	fscanf(fpInput, "%d\n", &testCase);

	for (int i = 0; i < testCase; i++)
	{
		// 전역변수 초기화
		treeDepthCache.clear();

		// Get the input 
		fscanf(fpInput, "%d", &treeNodeCount);
		treeDepthCache = vector<int>(treeNodeCount, -1);
		treeNodes = vector<TreeNode>(treeNodeCount);
		for (int index = 0; index < treeNodeCount; index++)
		{
			pair<int, int> treePos;
			int radius;
			fscanf(fpInput, "%d %d %d", &treePos.first, &treePos.second, &radius);
			treeNodes[index].nodeId = index;
			treeNodes[index].pos = treePos;
			treeNodes[index].radius = radius;
		}

		int root = formTreeRelation();
		int ret = maxTreeNodeDistance(root);
		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	//	fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif

	return 0;
}