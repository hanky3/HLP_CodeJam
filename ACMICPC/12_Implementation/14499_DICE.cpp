#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;



const pair<int, int> directions[4] = { { 0, 1 }, { 0, -1 }, { -1, 0 }, { 1, 0 } };
vector<int> dicePos = { 1, 0, 4, 5, 3, 2 };
const int diceRotation[][6] = {
	{ 0, 4, 2, 5, 3, 1 },
	{ 0, 5, 2, 4, 1, 3 },
	{ 1, 2, 3, 0, 4, 5 },
	{ 3, 0, 1, 2, 4, 5 },
};

int N, M, x, y, k;
vector<int> command;
vector<int> diceNum;
vector<vector<int>> map;

void readInputData()
{
	fscanf(fpInput, "%d %d %d %d %d", &N, &M, &x, &y, &k);
	map = vector<vector<int>>(N, vector<int>(M, 0));
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < M; w++) {
			fscanf(fpInput, "%d", &map[h][w]);
		}
	}
	command = vector<int>(k);
	for (int i = 0; i < k; i++) {
		fscanf(fpInput, "%d", &command[i]);
		command[i]--;
	}
	diceNum = vector<int>(6, 0);
}

void makeDiceRotation(vector<int> &rotation, int cmd)
{
	vector<int> newRotation(6);
	for (int pos = 0; pos < rotation.size(); pos++) {
		newRotation[pos] = rotation[diceRotation[cmd][pos]];
	}
	rotation = newRotation;
}

vector<int> findDiceTopNums()
{
	vector<int> ret;

	pair<int, int> curr(x, y);
	int currDicePos = 0;
	int index = 0;
	for (auto c : command) {
		pair<int, int> next(curr.first + directions[c].first, curr.second + directions[c].second);
		if (next.first < 0 || next.first >= N || next.second < 0 || next.second >= M)
			continue;
		// make rotation 
		makeDiceRotation(dicePos, c);
		if (map[next.first][next.second] == 0) {
			map[next.first][next.second] = diceNum[dicePos[3]];
		}
		else {
			diceNum[dicePos[3]] = map[next.first][next.second];
			map[next.first][next.second] = 0;
		}
		ret.push_back(diceNum[dicePos[1]]);
		curr = next;
	}

	return ret;
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
	while (testCase-- > 0) {
		readInputData();
		auto ret = findDiceTopNums();
		for (auto v : ret) {
#ifdef _FILE_
			printf("%d\n", v);
#endif
			fprintf(fpOutput, "%d\n", v);
		}
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
