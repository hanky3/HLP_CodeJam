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

int nodeCount;
int rootNode, removeNode;
vector<vector<int>> childNodes;
vector<int> parentNodes;
vector<int> nodeLeafCount;

void readInputData()
{
	fscanf(fpInput, "%d", &nodeCount);
	childNodes = vector<vector<int>>(nodeCount, vector<int>(0));
	parentNodes = vector<int>(nodeCount);
	nodeLeafCount = vector<int>(nodeCount, 0);
	for (int node = 0; node < nodeCount; node++) {
		int parent;
		fscanf(fpInput, "%d", &parent);
		parentNodes[node] = parent;
		if (parent == -1) {
			rootNode = node;
			continue;
		}
		childNodes[parent].push_back(node);
	}
	fscanf(fpInput, "%d", &removeNode);
}

int createLeafCount(int here)
{
	int leafCount = 0;
	if (childNodes[here].size() == 0) {
		nodeLeafCount[here] = 1;
		return 1;
	}

	for (auto child : childNodes[here]) {
		leafCount += createLeafCount(child);
	}
	nodeLeafCount[here] = leafCount;
	return leafCount;
}

int leafNodeCount(int root, int remove)
{
	int total = createLeafCount(root);
	int remainLeafNodes = (total - nodeLeafCount[remove]);
	int parent = parentNodes[remove];
	if (parent >= 0 && nodeLeafCount[parent] == nodeLeafCount[remove]) {
		remainLeafNodes++;
	}

	return remainLeafNodes;
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
		auto ret = leafNodeCount(rootNode, removeNode);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}