#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_WORD_SIZE		101

int H, W;
string word;
string reverseWord;
vector<vector<int>> map;

const pair<int, int> directions[4] = {
	{ 0, 1 }, { 1, 1 }, { 1, 0 }, { -1, 1 }
};

struct WordDirection {
	int step;
	pair<int, int> pos;
	int dir;
};

void readInputData()
{
	char buf[MAX_WORD_SIZE];
	fscanf(fpInput, "%s\n", buf);
	word = buf;
	reverseWord = word;
	reverse(reverseWord.begin(), reverseWord.end());

	fscanf(fpInput, "%d %d\n", &H, &W);

	map = vector<vector<int>>(H, vector<int>(W, 0));

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			char ch;
			fscanf(fpInput, "%c", &ch);
			map[h][w] = ch;
		}
		fscanf(fpInput, "\n");
	}
}


bool dfs(int wordPos, pair<int, int> pos, int dir, bool reverse)
{
	bool ret = false;

	char curr = word[wordPos];
	if (reverse)
		curr = reverseWord[wordPos];

	if (curr != map[pos.first][pos.second])
		return false;

	if (wordPos == word.size()-1)
		return true;

	pair<int, int> next(pos.first + directions[dir].first, pos.second + directions[dir].second);
	if (next.first < 0 || next.first >= H || next.second < 0 || next.second >= W)
		return false;

	ret = dfs(wordPos + 1, next, dir, reverse);
	return ret;
}

bool containWord()
{
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			for (int dir = 0; dir < 4; dir++) {
				if (dfs(0, make_pair(h, w), dir, false))
					return true;
				if (dfs(0, make_pair(h, w), dir, true))
					return true;
			}
		}
	}
	return false;
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
	while (testCase > 0)
	{
		readInputData();
		bool ret = containWord();
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
