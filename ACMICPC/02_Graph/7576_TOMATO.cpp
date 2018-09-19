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
vector<vector<int>> tomatoMap;

void readInputData()
{
	fscanf(fpInput, "%d %d\n", &M, &N);
	
	tomatoMap = vector<vector<int>>(N, vector<int>(M));
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			fscanf(fpInput, "%d", &tomatoMap[h][w]);
		}
	}
}

bool allTomatoRiped(vector<pair<int, int>> &ripedPos)
{
	int riped = 0;
	int blank = 0;
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			if (tomatoMap[h][w] == 1) {
				ripedPos.push_back({ h, w });
				riped++;
			}
			else if (tomatoMap[h][w] == -1)
				blank++;
		}
	}
	if (riped + blank == (N*M))
		return true;
	return false;
}

int minTomatoRipeDay()
{
	int minMoves = 0;
	vector<pair<int, int>> ripedPos;
	if (allTomatoRiped(ripedPos))
		return 0;

	queue<tuple<pair<int, int>, int>> q;
	for (auto pos : ripedPos)
		q.push(make_tuple(pos, 0));

	while (!q.empty()) {
		auto currPos = std::get<0>(q.front());
		auto moves = std::get<1>(q.front());

		q.pop();
		minMoves = max(minMoves, moves);

		const pair<int, int> directions[] = { { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 } };
		for (auto dir : directions) {
			auto nextPos = make_pair(currPos.first + dir.first, currPos.second + dir.second);
			if (nextPos.first < 0 || nextPos.first >= N || nextPos.second < 0 || nextPos.second >= M)
				continue;
			if (tomatoMap[nextPos.first][nextPos.second] == 0) {
				tomatoMap[nextPos.first][nextPos.second] = 1;
				q.push(make_tuple( nextPos, moves + 1 ));
			}
		}
	}

	if (allTomatoRiped(ripedPos))
		return minMoves;

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
		auto ret = minTomatoRipeDay();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

