#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <list>

using namespace std;
#define INF 0x0FFFFFFF
#define MAX_TREE_NODE 100

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

struct TreeNode {
	int nodeId;
	TreeNode *leftChildTree;
	TreeNode *rightChildTree;
};
int treeNodeCount;
TreeNode rootTree;
vector<int> preOrderTree;
vector<int> inOrderTree;


void releaseTree(TreeNode *root)
{
	if (root == NULL) return;

	if (root->leftChildTree) {
		releaseTree(root->leftChildTree);
		free(root->leftChildTree);
		root->leftChildTree = NULL;
	}
	if (root->rightChildTree) {
		releaseTree(root->rightChildTree);
		free(root->rightChildTree);
		root->rightChildTree = NULL;
	}
}


void postTreeTraverse(TreeNode *root, vector<int> &order)
{
	if (root == NULL) return;

	postTreeTraverse(root->leftChildTree, order);
	postTreeTraverse(root->rightChildTree, order);
	order.push_back(root->nodeId);
}

void formTreeWithOrder(TreeNode *root, vector<int> &preOrderTree, vector<int> &inOrderTree)
{
	// 루트 트리 설정
	root->nodeId = preOrderTree.front();
	root->leftChildTree = NULL;
	root->rightChildTree = NULL;
	preOrderTree.erase(preOrderTree.begin());

	// 서브트리 정보 없는 경우
	if (preOrderTree.size() == 0)
		return;

	// 왼쪽/오른쪽 서브트리 정렬순서 추출
	vector<int> leftInOrder, rightInOrder;
	bool isLeft = true;
	vector<int>::iterator itr = inOrderTree.begin();
	for (; itr != inOrderTree.end(); itr++)
	{
		if (*itr == root->nodeId)
		{
			isLeft = false;
			continue;
		}

		if (isLeft) 
			leftInOrder.push_back(*itr);
		else 
			rightInOrder.push_back(*itr);
	}

	// 서브트리 재구성
	if (leftInOrder.size() > 0) {
		root->leftChildTree = (TreeNode *)malloc(sizeof(TreeNode));
		formTreeWithOrder(root->leftChildTree, preOrderTree, leftInOrder);
	}

	if (rightInOrder.size() > 0) {
		root->rightChildTree = (TreeNode *)malloc(sizeof(TreeNode));
		formTreeWithOrder(root->rightChildTree, preOrderTree, rightInOrder);
	}
}

vector<int> postOrderTree()
{
	vector<int> ret;
	formTreeWithOrder(&rootTree, preOrderTree, inOrderTree);
	postTreeTraverse(&rootTree, ret);

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
		releaseTree(&rootTree);
		preOrderTree.clear();
		inOrderTree.clear();

		// Get the input 
		fscanf(fpInput, "%d", &treeNodeCount);
		preOrderTree = vector<int>(treeNodeCount);
		for (int index = 0; index < treeNodeCount; index++)
			fscanf(fpInput, "%d", &preOrderTree[index]);

		inOrderTree = vector<int>(treeNodeCount);
		for (int index = 0; index < treeNodeCount; index++)
			fscanf(fpInput, "%d", &inOrderTree[index]);

		vector<int> ret = postOrderTree();
		for (int index = 0; index < treeNodeCount; index++)
		{
			fprintf(fpOutput, "%d ", ret[index]);
			if (isFile)
				printf("%d ", ret[index]);
		}

		fprintf(fpOutput, "\n");
		if (isFile)
			printf("\n");

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