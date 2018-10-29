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

#define MAXN    (100000 + 1)



// 트리정보
struct TreeNode 
{
	int index;  // start
	int depth;  // distance from root
	int parent; // parent
	vector<int> childNodes;
};
int treeNodeCount;
vector<pair<int, int>> treeParentInfo;
vector<TreeNode> treeNodes;
int questionCount;

// 트리의 Depth 설정
void setTreeDepth(int node, int currentDepth)
{
	treeNodes[node].depth = currentDepth;
	for (int child = 0; child < treeNodes[node].childNodes.size(); child++)
		setTreeDepth(treeNodes[node].childNodes[child], currentDepth + 1);
}

// 조상을 빠르게 찾기 위한 lookup table
#define LOG 20  // >= log2(MAXN)
int cmnAncestorTable[MAXN][LOG];
void makeAncestorLookupTable()
{
	//-- 조상을 빠르게 찾기 위한 lookup table을 만든다.
	memset(cmnAncestorTable, 0, sizeof(cmnAncestorTable));
	for (int node = 0; node < treeNodeCount; node++) {
		cmnAncestorTable[node][0] = treeNodes[node].parent;
	}
	for (int i = 1; i < LOG; i++) {
		for (int node = 0; node < treeNodeCount; node++) {
			int parentNode = cmnAncestorTable[node][i - 1];
			cmnAncestorTable[node][i] = cmnAncestorTable[parentNode][i - 1];
		}
	}
}

void makeTree(int treeNodeCount)
{
	// 트리의 관계 설정(부모 자식)
	treeNodes = vector<TreeNode>(treeNodeCount);
	treeNodes[0].parent = 0;
	treeNodes[0].index = 0;

	for (int node = 1; node < treeNodeCount; node++)
	{
		fscanf(fpInput, "%d", &treeNodes[node].parent);

		int parentNode = treeNodes[node].parent;
		treeNodes[node].index = node;
		treeNodes[parentNode].childNodes.push_back(node);
	}

	// 트리의 depth 설정
	setTreeDepth(0, 0);
	//자식 노드 정보를 바탕으로해서 모든 노드의 depth과 parent를 설정한다
	makeAncestorLookupTable();
}

int climbTree(int node, int distance)
{
	if (distance <= 0)
		return node;

	if (distance >= treeNodes[node].depth)
		return 0;

	for (int index = 0; distance > 0; index++) {
		if (distance & 1)
			node = cmnAncestorTable[node][index];
		distance >>= 1;
	}

	return node;
}

int treeDistance(pair<int, int> &nodes)
{
	int lowNode = treeNodes[nodes.first].depth < treeNodes[nodes.second].depth ? nodes.first : nodes.second;
	int highNode = treeNodes[nodes.first].depth < treeNodes[nodes.second].depth ? nodes.second : nodes.first;

	int highParentNode = climbTree(highNode, treeNodes[highNode].depth - treeNodes[lowNode].depth);

	if (highParentNode == lowNode)
		return treeNodes[highNode].depth - treeNodes[lowNode].depth;

	int parentNode;
	int index;
	for (index = 0; index < LOG; index++) {
		if (cmnAncestorTable[highParentNode][index] == cmnAncestorTable[lowNode][index])
			break;
	}

	parentNode = cmnAncestorTable[highParentNode][0];
	if (index > 0)
	{
		int loS = cmnAncestorTable[highParentNode][index - 1];
		int hiS = cmnAncestorTable[highParentNode][index];
		int loE = cmnAncestorTable[lowNode][index - 1];
		int hiE = cmnAncestorTable[lowNode][index];

		while (treeNodes[loS].depth > treeNodes[hiS].depth) {
			int midDepth = (treeNodes[loS].depth + treeNodes[hiS].depth + 1) / 2;

			int midS = climbTree(loS, treeNodes[loS].depth - midDepth);
			int midE = climbTree(loE, treeNodes[loE].depth - midDepth);

			if (midS == midE) {
				hiS = midS;
				hiE = midE;
			}
			else {
				loS = treeNodes[midS].parent;
				loE = treeNodes[midE].parent;
			}
		}
		parentNode = loS;
	}

	int ret = (treeNodes[nodes.first].depth - treeNodes[parentNode].depth) +
		(treeNodes[nodes.second].depth - treeNodes[parentNode].depth);

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
		// Get the input 
		fscanf(fpInput, "%d %d", &treeNodeCount, &questionCount);

		// 트리 관계를 설정한다.
		makeTree(treeNodeCount);

		for (int index = 0; index < questionCount; index++)
		{
			pair<int, int> nodePairs;
			fscanf(fpInput, "%d %d", &nodePairs.first, &nodePairs.second);
			int ret = treeDistance(nodePairs);
			fprintf(fpOutput, "%d\n", ret);
			if (isFile)
				printf("%d\n", ret);

		}
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