#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <time.h>
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
#define BOARD_LINES				5
#define ALPHABET_CNT			26
#define ADJ_DIRECTION_CNT		8
#define MAX_KEYWORD_CNT	10

char gameBoard[BOARD_LINES][BOARD_LINES+1];
vector<pair<int, int>> boardPosition[ALPHABET_CNT];

const pair<int, int> adjDirections[ADJ_DIRECTION_CNT] = {
	make_pair(0, 1), make_pair(0, -1),
	make_pair(1, 1), make_pair(1, 0), make_pair(1, -1),
	make_pair(-1, 1), make_pair(-1, 0), make_pair(-1, -1),
};

bool isValidBoardPos(pair<int, int> currPos)
{
	if (currPos.first < 0 || currPos.second < 0 || currPos.first >= BOARD_LINES || currPos.second >= BOARD_LINES)
		return false;

	return true;
}

int cacheFinder[BOARD_LINES][BOARD_LINES][MAX_KEYWORD_CNT+1];
int canFindKeyword(string keyword, pair<int, int> &currPos)
{
	if (!isValidBoardPos(currPos))
		return false;

//	if (boardVisited[currPos.second][currPos.first])
//		return false;

	if (gameBoard[currPos.second][currPos.first] != keyword.front())
		return false;

	int &ret = cacheFinder[currPos.second][currPos.first][keyword.length()];
	if (ret >= 0)
		return ret;

	keyword.erase(keyword.begin());
	if (keyword.empty())
		return true;

	ret = 0;
	for (int adj = 0; adj < ADJ_DIRECTION_CNT; adj++)
	{
		pair<int, int> newPos;
		newPos.first = currPos.first + adjDirections[adj].first;
		newPos.second = currPos.second + adjDirections[adj].second;

		if (canFindKeyword(keyword, newPos)) {
			ret = 1;
			break;
		}
	}
	/*
	vector<pair<int, int>> adjPositions = adjBoardPosition[keyword[0] - 'A'][currPos.second][currPos.first];
	for (int index = 0; index < adjPositions.size(); index++)
	{
		pair<int, int> &newPos = adjPositions[index];
		if (canFindKeyword(keyword, newPos)) {
			ret = 1;
			break;
		}
	}
	*/
	return ret;
}

void setGameBoard()
{
	for (int index = 0; index < ALPHABET_CNT; index++)
	{
		boardPosition[index].clear();
	}

	for (int y = 0; y < BOARD_LINES; y++)
	{
		fscanf(fpInput, "%s", gameBoard[y]);
		for (int x = 0; x < BOARD_LINES; x++)
			boardPosition[gameBoard[y][x] - 'A'].push_back(make_pair(x, y));
	}
}


void findKeywordPaths(bool isFile)
{
	char keyword[11];
	int keywordCnt;

	fscanf(fpInput, "%d", &keywordCnt);
	for (int index = 0; index < keywordCnt; index++)
	{
		string ret = "NO";
		fscanf(fpInput, "%s", keyword);
		memset(cacheFinder, -1, sizeof(int)*BOARD_LINES*BOARD_LINES*(MAX_KEYWORD_CNT+1));

		vector<pair<int, int>> firstPositions = boardPosition[keyword[0] - 'A'];
		for (int posIndex = 0; posIndex < firstPositions.size(); posIndex++)
		{
			if (canFindKeyword(string(keyword), firstPositions[posIndex]) > 0)
			{
				ret = "YES";
				break;
			}
		}

		fprintf(fpOutput, "%s %s\n", keyword, ret.c_str());
		if (isFile)
			printf("%s %s\n", keyword, ret.c_str());
	}
}


void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");

	if (!isFile)
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else
	{
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++)
	{
		// 전역변수 초기화
		setGameBoard();
		findKeywordPaths(isFile);
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}