#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N, K, M;
vector<vector<int>> connectedInfo;
vector<vector<int>> stationTubes;

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &N, &K, &M);

	connectedInfo = vector<vector<int>>(M, vector<int>(K));
	stationTubes = vector<vector<int>>(N);

	for (int t = 0; t < M; t++) {
		for (int s = 0; s < K; s++) {
			fscanf(fpInput, "%d", &connectedInfo[t][s]);
			connectedInfo[t][s]--;
			stationTubes[connectedInfo[t][s]].push_back(t);
		}
	}
}


int findShortestPath(int start, int end)
{
	queue<pair<pair<int, int>, int>> q;

	vector<int> tubes = stationTubes[start];
	for (auto tube : tubes)
		q.push(make_pair(make_pair(tube, 0), 1));

	vector<bool> visited(M, 0);
	while (!q.empty()) {
		int hereTube = q.front().first.first;
		int hereStation = q.front().first.second;
		int dist = q.front().second;
		q.pop();

		if (visited[hereTube]) continue;
		visited[hereTube] = true;
		if (hereStation == end)
			return dist;

		auto itr = find(connectedInfo[hereTube].begin(), connectedInfo[hereTube].end(), end);
		if (itr != connectedInfo[hereTube].end())
			return (dist+1);

		for (auto there : connectedInfo[hereTube]) {
			if (there != hereStation) {
				for (auto thereTube : stationTubes[there])
					q.push(make_pair(make_pair(thereTube, there), dist + 1));
			}
		}
	}
	return -1;
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

		auto ret = findShortestPath(0, N-1);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}