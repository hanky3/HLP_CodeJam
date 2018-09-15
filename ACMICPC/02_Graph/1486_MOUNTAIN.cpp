#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <limits>
#include <queue>
#include <functional>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

#define ALPHABET_SIZE		26
#define INF	987654321

int N, M, T, D;
vector<vector<int>> mountainHeight;
vector<vector<int>> distances;
const pair<int, int> directions[] = { {-1, 0}, {1, 0}, {0, -1},{ 0, 1 } };


int getHeight(char code)
{
	if (code >= 'a' && code <= 'z')
		return code - 'a' + ALPHABET_SIZE;
	return code - 'A';
}

void readInputData()
{
	fscanf(fpInput, "%d %d %d %d\n", &N, &M, &T, &D);
	mountainHeight = vector<vector<int>>(N, vector<int>(M));
	distances = vector<vector<int>>(N*M, vector<int>(N*M, INF));

	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			char code;
			fscanf(fpInput, "%c", &code);
			mountainHeight[h][w] = getHeight(code);
		}
		fscanf(fpInput, "\n");
	}
}

void dijkstra(int src, vector<int> &dist)
{
	priority_queue < pair<int, int>, vector<pair<int, int>>,
		std::greater<pair<int, int>>> q;

	if (dist[src] == 0)
		return;

	dist[src] = 0;
	q.push(make_pair(0, src));
	while (!q.empty()) {
		auto here = q.top().second;
		pair<int, int> herePos(here / M, here%M);
		int cost = q.top().first;
		q.pop();

		if (dist[here] < cost)
			continue;

		for (int dir = 0; dir < 4; dir++) {
			pair<int, int> there(herePos.first + directions[dir].first, herePos.second + directions[dir].second);
			if (there.first < 0 || there.first >= N || there.second < 0 || there.second >= M)
				continue;

			int heightDiff = abs(mountainHeight[herePos.first][herePos.second] - mountainHeight[there.first][there.second]);
			if (heightDiff <= T) {
				int nextCost = cost + 1;
				if (mountainHeight[there.first][there.second] > mountainHeight[herePos.first][herePos.second]) {
					nextCost = cost + (int)pow(heightDiff, 2);
				}

				int thereId = there.first*M + there.second;
				if (nextCost < dist[thereId]) {
					dist[thereId] = nextCost;
					q.push({ nextCost, thereId });
				}
			}
		}
	}
}


int findMaxHeight()
{
	int maxHeight = 0;
	maxHeight = mountainHeight[0][0];

	int totalNode = (N*M);
	for (int n = 0; n < totalNode; n++)
		dijkstra(n, distances[n]);

	for (int n = 0; n < totalNode; n++) {
		if (distances[0][n] + distances[n][0] <= D) {
			maxHeight = max(maxHeight, mountainHeight[n / M][n%M]);
		}
	}

	return maxHeight;
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
		auto ret = findMaxHeight();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
