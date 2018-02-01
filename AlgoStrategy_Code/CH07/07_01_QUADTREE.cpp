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
#define MAX_WORD_CNT		1000

//vector<vector<bool>> gameBoards;
int wordCount;
char quadTreeWord[MAX_WORD_CNT + 1];

string reverseQuadTree(string &origin)
{
	char word = origin.front();
	origin.erase(origin.begin());

	string ret;
	if (word != 'x')
	{
		ret.push_back(word);
		return ret;
	}

	string topLeft = reverseQuadTree(origin);
	string topRight = reverseQuadTree(origin);
	string bottomLeft = reverseQuadTree(origin);
	string bottomRight = reverseQuadTree(origin);

	ret = "x" + bottomLeft;
	ret.append(bottomRight);
	ret.append(topLeft);
	ret.append(topRight);

	return ret;
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
		memset(quadTreeWord, 0, sizeof(quadTreeWord));
		fscanf(fpInput, "%s", quadTreeWord);

		string quadWords = string(quadTreeWord);
		string ret = reverseQuadTree(quadWords);
		fprintf(fpOutput, "%s\n", ret.c_str());
		if (isFile)
			printf("%s\n", ret.c_str());
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
