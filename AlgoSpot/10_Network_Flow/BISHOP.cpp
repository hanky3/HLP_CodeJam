#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <queue>
#include <assert.h>


using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_W	510
#define MAX_H		510
#define MY_TURN_SET	0
typedef unsigned long long uint64;

int N;
pair<int, int> boardBlocks[MAX_H][MAX_W];
bool adj[MAX_H][MAX_W];
vector<bool> visited;
vector<int> aMatch;
vector<int> bMatch;
pair<int, int> bishopIndex;
int n, m;

const pair<int, int> directions[] = {
	make_pair(-1, -1),  make_pair(1, 1), make_pair(-1, 1), make_pair(1, -1)
};

void readInputData()
{
	//memset(boardBlocks, -1, sizeof(boardBlocks));
	memset(adj, 0, sizeof(adj));
	fscanf(fpInput, "%d\n", &N);

	for (int h = 0; h < N; h++) {
		for (int w = 0; w < N; w++) {
			char ch;
			fscanf(fpInput, "%c", &ch);
			boardBlocks[h][w] = (ch == '*') ? make_pair(0, 0) : make_pair(-1, -1);
		}
		fscanf(fpInput, "\n");
	}
	bishopIndex = make_pair(1, 1);
}

bool dfs(int a) 
{
	if (visited[a])
		return false;

	visited[a] = true;
	for (int b = 0; b < bMatch.size(); b++) {
		if (adj[a][b]) {
			if (bMatch[b] == -1 || dfs(bMatch[b])) {
				aMatch[a] = b;
				bMatch[b] = a;
				return true;
			}
		}
	}
	return false;
}

int bipartiteMatch(int aCount, int bCount)
{
	aMatch = vector<int>(aCount, -1);
	bMatch = vector<int>(bCount, -1);

	int size = 0;
	for (int start = 0; start < aCount; start++) {
		visited = vector<bool>(aCount, false);
		if (dfs(start))
			size++;
	}
	return size;
}

void updateBishopMatching(pair<int, int> pos)
{
	auto currBoard = boardBlocks[pos.first][pos.second];
	if (currBoard.first >= 0 && currBoard.second >= 0)
		return;

	for (int i = 0; i < 4; i++) {
		int start = 1;

		while (true) {
			pair<int, int> next = make_pair(pos.first + (start*directions[i].first), pos.second + (start*directions[i].second));

			if (next.first < 0 || next.second < 0 || next.first >= N || next.second >= N)
				break;

			int &aIndex = boardBlocks[next.first][next.second].first;
			int &bIndex = boardBlocks[next.first][next.second].second;
			if (i < 2) {
				if (aIndex >= 0) break;
				aIndex = bishopIndex.first;
			}
			else {
				if (bIndex >= 0) break;
				bIndex = bishopIndex.second;
			}

			if (aIndex > 0 && bIndex > 0) {
				adj[aIndex][bIndex] = true;
			}

			start++;
		}
	}

	if (boardBlocks[pos.first][pos.second].first < 0)
		boardBlocks[pos.first][pos.second].first = bishopIndex.first++;
	if (boardBlocks[pos.first][pos.second].second < 0)
		boardBlocks[pos.first][pos.second].second = bishopIndex.second++;
	adj[boardBlocks[pos.first][pos.second].first][boardBlocks[pos.first][pos.second].second] = true;
}


int placeMaxBishop()
{
	int ret = 0;

	for (int h = 0; h < N; h++) {
		for (int w = 0; w < N; w++) {
			updateBishopMatching(make_pair(h, w));
		}
	}

	ret = bipartiteMatch(bishopIndex.first, bishopIndex.second);
	return ret;
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

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		int ret = placeMaxBishop();
		if (isFile)
			printf("%d\n", ret);
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
