#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <math.h>


using namespace std;
typedef long long int64;
FILE *fpInput;
FILE *fpOutput;

int N, M;
int dirtyCounter;
vector<vector<char>> roomInfo;
map<pair<int, int>, int> dirtyPosIndex;
pair<int, int> startPos;

void readInputData()
{
	fscanf(fpInput, "%d %d\n", &M, &N);
	if (N == 0 && M == 0) return;

	dirtyCounter = 0;
	roomInfo = vector<vector<char>>(N, vector<char>(M));
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			fscanf(fpInput, "%c", &roomInfo[h][w]);
			if (roomInfo[h][w] == '*')
				dirtyPosIndex[{h, w}] = dirtyCounter++;
			else if (roomInfo[h][w] == 'o')
				startPos = { h, w };
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

int minRobotMoves()
{
	int minMoves = numeric_limits<int>::max();
	queue<tuple<pair<int, int>, int, int>> q;
	vector<vector<vector<int>>> visited(N, vector<vector<int>>(M, vector<int>()));
	q.push(make_tuple(startPos, 0, 0));

	if (dirtyCounter == 0) return 0;

	int allCleaned = (1 << dirtyCounter) - 1;
	while (!q.empty()) {
		auto currPos = std::get<0>(q.front());
		auto moves = std::get<1>(q.front());
		auto cleaned = std::get<2>(q.front());

		if (moves > (N*M)) continue;
		q.pop();

		if (roomInfo[currPos.first][currPos.second] == '*') {
			cleaned |= (1 << dirtyPosIndex[currPos]);
		}

		if ((cleaned & allCleaned) == allCleaned) {
			return moves;
		}

		const pair<int, int> directions[] = { { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 } };
		for (auto dir : directions) {
			auto nextPos = make_pair(currPos.first + dir.first, currPos.second + dir.second);
			if (!isGameBoundary(nextPos)) continue;
			if (roomInfo[nextPos.first][nextPos.second] == 'x') continue;

			int nextMove = moves + 1;
			bool duplicated = false;
			for (auto v : visited[nextPos.first][nextPos.second]) {
				if (v == cleaned) {
					duplicated = true;
					break;
				}
			}

			if (duplicated) continue;

			q.push(make_tuple(nextPos, nextMove, cleaned));
			visited[nextPos.first][nextPos.second].push_back(cleaned);
		}
	}
	if (minMoves == numeric_limits<int>::max())
		return -1;

	return minMoves;
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
	while (true) {
		readInputData();
		if (N == 0 && M == 0)
			break;

		auto ret = minRobotMoves();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

