#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <math.h>


using namespace std;

#define MAX_H			50
#define MAX_W		50

FILE *fpInput;
FILE *fpOutput;

int L, W;
bool map[MAX_H][MAX_W];
vector<pair<int, int>> startPosList;

const pair<int, int> directions[4] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

void readInputData()
{
	memset(map, 0, sizeof(map));
	startPosList.clear();
	fscanf(fpInput, "%d %d", &L, &W);
	char buf[MAX_W];
	for (int h = 0; h < L; h++) {
		fscanf(fpInput, "%s", buf);
		for (int w = 0; w < W; w++) {
			if (buf[w] == 'L') {
				map[h][w] = true;
				startPosList.push_back(make_pair(h, w));
			}
		}
	}
}

int bfs(int h, int w)
{
	int distance = 0;
	queue<pair<pair<int, int>, int>> q;
	q.push(make_pair(make_pair(h, w), 0));

	vector<vector<bool>> visited(L, vector<bool>(W, 0));
	while (!q.empty()) {
		pair<int, int> here = q.front().first;
		int cost = q.front().second;
		q.pop();

		if (visited[here.first][here.second]) continue;
		visited[here.first][here.second] = true;
		distance = max(distance, cost);

		for (auto dir : directions) {
			pair<int, int> there(here.first + dir.first, here.second + dir.second);
			if (there.first < 0 || there.first >= L || there.second < 0 || there.second >= W)
				continue;
			if (!map[there.first][there.second])
				continue;

			q.push(make_pair(there, cost + 1));
		}

	}
	return distance;
}

int findShortestPath()
{
	int pathDist = 0;
	for (auto start : startPosList) {
		pathDist = max(pathDist, bfs(start.first, start.second));
	}
	return pathDist;
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
		auto ret = findShortestPath();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
