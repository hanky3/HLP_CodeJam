#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <math.h>

using namespace std;
typedef long long int64;
FILE *fpInput;
FILE *fpOutput;

int N, M;
int counter;
vector<vector<char>> gameMap;
vector<vector<int>> discovered;
vector<vector<bool>> finished;
int maxCoinMoves;
bool infiniteMoves;

void readInputData()
{
	fscanf(fpInput, "%d %d\n", &N, &M);
	counter = 0;
	maxCoinMoves = 0;
	infiniteMoves = false;
	discovered = vector<vector<int>>(N, vector<int>(M, -1));
	finished = vector<vector<bool>>(N, vector<bool>(M, false));
	gameMap = vector<vector<char>>(N, vector<char>(M));
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			fscanf(fpInput, "%c", &gameMap[h][w]);
		}
		fscanf(fpInput, "\n");
	}
}

bool isGameBoundary(pair<int, int> pos)
{
	if (pos.first < 0 || pos.first >= N || pos.second < 0 || pos.second >= M) {
		return false;
	}
	return true;
}

int findCoinMaxMoves()
{
	int maxMoves = 0;
	queue<tuple<pair<int, int>, int>> q;
	vector<vector<int>> dist(N, vector<int>(M, -1));
	q.push(make_tuple(make_pair(0, 0), 0));

	while (!q.empty()) {
		auto currPos = std::get<0>(q.front());
		auto moves = std::get<1>(q.front());

		if (moves > (N*M))
			return -1;
		q.pop();

		if (gameMap[currPos.first][currPos.second] == 'H') {
			maxMoves = max(maxMoves, moves);
			continue;
		}

		int step = gameMap[currPos.first][currPos.second] - '0';
		const pair<int, int> directions[] = { { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 } };
		for (auto dir : directions) {
			auto nextPos = make_pair(currPos.first + (step*dir.first), currPos.second + (step*dir.second));
			if (!isGameBoundary(nextPos)) {
				maxMoves = max(maxMoves, moves+1);
				continue;
			}
			if (dist[nextPos.first][nextPos.second] <= moves) {
				q.push(make_tuple(nextPos, moves + 1));
				dist[nextPos.first][nextPos.second] = moves + 1;
			}
		}
	}


	return maxMoves;
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
		auto ret = findCoinMaxMoves();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}