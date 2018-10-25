#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int J, N;
vector<pair<int, int>> rockPosList;
pair<int, int> startRock, endRock;
vector<vector<int>> adjRockList;

void readInputData()
{
	fscanf(fpInput, "%d", &J);
	fscanf(fpInput, "%d %d", &startRock.first, &startRock.second);
	fscanf(fpInput, "%d %d", &endRock.first, &endRock.second);
	fscanf(fpInput, "%d", &N);

	rockPosList = vector<pair<int, int>>(N + 2);
	rockPosList[0] = startRock;
	rockPosList[N+1] = endRock;

	adjRockList = vector<vector<int>>(N + 2, vector<int>());

	for (int here = 1; here <= N+1; here++) {
		if (here <= N)
			fscanf(fpInput, "%d %d", &rockPosList[here].first, &rockPosList[here].second);
		for (int there = 0; there < here; there++) {
			double dist = sqrt(pow(abs(rockPosList[here].first - rockPosList[there].first), 2) + pow(abs(rockPosList[here].second - rockPosList[there].second), 2));
			if (dist <= (double)J) {
				adjRockList[here].push_back(there);
				adjRockList[there].push_back(here);
			}
		}
	}
}

string escapeRiver()
{
	vector<bool> visited(N+2, false);
	queue<int> q;
	q.push(0);

	while (!q.empty()) {
		int curr = q.front();
		q.pop();

		if (visited[curr])	continue;
		if (curr == (N + 1))	return "YES";
		visited[curr] = true;

		for (auto adj : adjRockList[curr]) {
			q.push(adj);
		}
	}

	return "NO";
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
		auto ret = escapeRiver();
		if (isFile)
			printf("%s\n", ret.c_str());
		fprintf(fpOutput, "%s\n", ret.c_str());
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