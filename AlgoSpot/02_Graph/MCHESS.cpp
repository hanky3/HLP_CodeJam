#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

#define MAX_TEXT_SIZE	101
typedef pair<int, int> _pos;

FILE *fpInput;
FILE *fpOutput;

int N;
vector<vector<char>> chessMap;
_pos startPos;
_pos endPos;
map<char, vector<_pos>> chessDirections;
map<char, bool> chessDirOnce;
vector<vector<bool>> visited;
// P B N R Q K

void initConstValue()
{
	chessDirOnce.insert(make_pair('P', true));
	chessDirOnce.insert(make_pair('B', false));
	chessDirOnce.insert(make_pair('N', true));
	chessDirOnce.insert(make_pair('R', false));
	chessDirOnce.insert(make_pair('Q', false));
	chessDirOnce.insert(make_pair('K', true));

	vector<_pos> posList = {{-1,-1}, {-1,1}, {1,-1}, {1,1}, {0,1}, {0,-1}, {-1,0}, {1,0}};
	chessDirections.insert(make_pair('K', posList));
	chessDirections.insert(make_pair('Q', posList));

	posList = { {1,0} };
	chessDirections.insert(make_pair('P', posList));
	posList = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };
	chessDirections.insert(make_pair('B', posList));
	posList = { {1,2}, {1,-2}, {-1,2}, {-1,-2}, {2,-1}, {2,1}, {-2,-1}, {-2,1} };
	chessDirections.insert(make_pair('N', posList));
	posList = { {0,1}, {0,-1}, {-1,0}, {1,0} };
	chessDirections.insert(make_pair('R', posList));
}

void readInputData()
{
	fscanf(fpInput, "%d\n", &N);
	chessMap = vector<vector<char>>(N + 1, vector<char>(N + 1, 0));
	visited = vector<vector<bool>>(N + 1, vector<bool>(N + 1, false));

	fscanf(fpInput, "%d %d %d %d\n", &startPos.first, &startPos.second, 
										&endPos.first, &endPos.second);

	for (int h = N; h >= 1; h--) {
		for (int w = 1; w <= N; w++) {
			fscanf(fpInput, "%c", &chessMap[h][w]);
		}
		fscanf(fpInput, "\n");
	}
}

void candidateMoves(int step, _pos curr, char type, queue<pair<int, pair<_pos, char>>> &q)
{
	if (type == '.')
		return;

	vector<_pos> directions = chessDirections[type];
	bool once = chessDirOnce[type];

	for (auto dir : directions) {
		int m = 1;
		while (true) {
			_pos next(curr.first + (dir.first*m), curr.second + (dir.second*m));
			if (next.first <= 0 || next.first > N || next.second <= 0 || next.second > N)
				break;

			if (!visited[next.first][next.second])
				q.push(make_pair(step+1, make_pair(next, type)));

			if (once)
				break;

			m++;
		}
	}
}

int minMovesToDest()
{
	int ret = 0;
	queue<pair<int, pair<_pos, char>>> q;

	q.push(make_pair(0, make_pair(startPos, 'K')));
	while (true) {
		int step = q.front().first;
		_pos currPos = q.front().second.first;
		char type = q.front().second.second;
		q.pop();

		if (endPos == currPos)
			return step;

		if (currPos.first <= 0 || currPos.first > N || currPos.second <= 0 || currPos.second > N)
			continue;
		if (visited[currPos.first][currPos.second])
			continue;

		visited[currPos.first][currPos.second] = true;

		candidateMoves(step, currPos, type, q);
		char newType = chessMap[currPos.first][currPos.second];
		if (type != newType)
			candidateMoves(step, currPos, newType, q);
	}

	return -1;
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

	initConstValue();
	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0) {
		readInputData();
		int ret = minMovesToDest();
		if (isFile) {
			printf("%d\n", ret);
		}
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