#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define MOD	20130728

FILE *fpInput;
FILE *fpOutput;

int N, M;
vector<vector<bool>> adjInfo;
vector<vector<int>> adjConnectedCnt;
vector<pair<int, int>> edgeInfo;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);

	adjInfo = vector<vector<bool>>(N, vector<bool>(N, false));
	edgeInfo = vector<pair<int, int>>(M);
	adjConnectedCnt = vector<vector<int>>(N, vector<int>(N, 0));

	for (int index = 0; index < M; index++) {
		fscanf(fpInput, "%d %d", &edgeInfo[index].first, &edgeInfo[index].second);
		edgeInfo[index].first--;
		edgeInfo[index].second--;
		adjInfo[edgeInfo[index].first][edgeInfo[index].second] = true;
		adjInfo[edgeInfo[index].second][edgeInfo[index].first] = true;
	}
}

void calcConnectedNode()
{
	for (int node1 = 0; node1 < N; node1++) {
		for (int node2 = node1 + 1; node2 < N; node2++) {
			if (!adjInfo[node1][node2]) 
				continue;
			for (int node3 = node1 + 1; node3 < N; node3++) {
				if (!adjInfo[node2][node3]) 
					continue;
				adjConnectedCnt[node1][node3]++;
			}
		}
	}
}


int fourNodeCycles()
{
	int ret = 0;

	for (int node1 = 0; node1 < N; node1++) {
		for (int node2 = node1 + 1; node2 < N; node2++) {
			int connected = adjConnectedCnt[node1][node2];
			ret += (connected * (connected - 1) / 2);
			ret %= MOD;
		}
	}

	return ret;
}



void solveProblem(const char *fileName, bool isFile)
{
	fpInput = stdin;
	fpOutput = stdout;
	if (isFile) {
		fpInput = fopen(fileName, "r");
		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		calcConnectedNode();
		int ret = fourNodeCycles();
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif
	return 0;
}