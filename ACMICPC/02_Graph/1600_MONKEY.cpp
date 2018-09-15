#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <queue>
#include <vector>
#include <tuple>
#include <algorithm>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int K, W, H;
vector<vector<int>> map;

const pair<int, int> hDirections[] = { {-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {2,-1}, {2,1}, {1,-2}, {1,2} };
const pair<int, int> mDirections[] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } };

void readInputData()
{
	fscanf(fpInput, "%d", &K);
	fscanf(fpInput, "%d %d", &W, &H);

	map = vector<vector<int>>(H, vector<int>(W, 0));
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			fscanf(fpInput, "%d", &map[h][w]);
		}
	}
}

bool canGoNext(pair<int, int> &pos)
{
	if (pos.first < 0 || pos.first >= H || pos.second < 0 || pos.second >= W)
		return false;
	if (map[pos.first][pos.second] == 1)
		return false;

	return true;
}

int minMonkeyMove()
{
	queue<tuple<pair<int, int>, int, int>> q;
	vector<vector<int>> dist(H, vector<int>(W, numeric_limits<int>::max()));
	q.push(make_tuple(make_pair(0, 0), 0, 0));

	while (!q.empty()) {
		auto currPos = std::get<0>(q.front());
		auto moves = std::get<1>(q.front());
		auto hMoves = std::get<2>(q.front());
		q.pop();

		if (dist[currPos.first][currPos.second] < hMoves)
			continue;

		if (currPos.first == (H - 1) && currPos.second == (W - 1)) {
			return moves;
		}
		
		if (hMoves < K) {
			for (auto dir : hDirections) {
				auto nextPos = make_pair(currPos.first + dir.first, currPos.second + dir.second);
				if (!canGoNext(nextPos)) continue;
				if (dist[nextPos.first][nextPos.second] > (hMoves+1)) {
					q.push(make_tuple(nextPos, moves + 1, hMoves + 1));
					dist[nextPos.first][nextPos.second] = hMoves + 1;
				}
			}
		}
		for (auto dir : mDirections) {
			auto nextPos = make_pair(currPos.first + dir.first, currPos.second + dir.second);
			if (!canGoNext(nextPos)) continue;
			if (dist[nextPos.first][nextPos.second] > hMoves) {
				q.push(make_tuple(nextPos, moves + 1, hMoves));
				dist[nextPos.first][nextPos.second] = hMoves;
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
		auto ret = minMonkeyMove();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
