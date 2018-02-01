#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <deque>

#if 0
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

#define MAX_LINE_NUM		10000
#define MAX_SHEEP_NUM	10000
#define PRE_CALC_NUM		10


#define IS_BIT_SET(IDX)	(1 << IDX)
#define ALL_FLAGS			(1 << 5 - 1)
#define FLAG_SIZE			(1 << 5)

#define ONE_FLAG			(0)
#define TWO_FLAG_1		(1)
#define TWO_FLAG_2		(2)
#define THREE_FLAG_1	(3)
#define THREE_FLAG_2	(4)
#define SPECIAL_CHAR_SIZE	(5)

struct specialChar
{
	int lineNum;
	int realnum;
};

const struct specialChar SPECIAL_CHARS[SPECIAL_CHAR_SIZE] = {
	{ 1, 1 }, { 2, 5 }, { 2, 10 }, { 3, 4 }, {3, 9}
};


bool canCountSheep(int lineNum, int sheepCount, int specialNum);


bool iterateSpecialChars(int lineNum, int sheepCount, int specialNum)
{
	int lineSize = SPECIAL_CHARS[specialNum].lineNum;
	int realNum = SPECIAL_CHARS[specialNum].realnum;

	bool ret = false;
	int left = 0;
	while (true)
	{
		int remainLines = lineNum - left*lineSize;
		int remainSheep = sheepCount - left*realNum;

		if (remainLines < 0 || remainSheep < 0)
			break;

		if (canCountSheep(remainLines, remainSheep, specialNum + 1))
			return true;

		left++;
	}
	return false;
}

char cacheSheepNum[MAX_LINE_NUM+1][MAX_SHEEP_NUM+1][SPECIAL_CHAR_SIZE + 1];
bool canCountSheep(int lineNum, int sheepCount, int specialNum)
{
	char &ret = cacheSheepNum[lineNum][sheepCount][specialNum];
	if (ret != -1)
		return ret > 0 ? true : false;

	ret = 0;
	if (lineNum == sheepCount)
	{
		ret = 1;
		return true;
	}

	if (lineNum > sheepCount)
		return false;

	if (specialNum == SPECIAL_CHAR_SIZE)
	{
		if (lineNum == 0 && sheepCount == 0)
			ret = 1;
		return ret > 0 ? true : false;
	}

	if (iterateSpecialChars(lineNum, sheepCount, specialNum))
		ret = 1;

	return ret > 0 ? true : false;

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
//	preCalcPossibleNums();

	printf("cached..\n");
	for (int i = 0; i < testcase; i++)
	{
		// 전역변수 초기화
		memset(cacheSheepNum, -1, sizeof(cacheSheepNum));

		int sheepCount, lineNum;
		fscanf(fpInput, "%d %d", &sheepCount, &lineNum);

		bool ret = canCountSheep(lineNum, sheepCount, 0);
		string resultStr = "X";
		if (ret)
			resultStr = "O";

		fprintf(fpOutput, "%s\n", resultStr.c_str());
		if (isFile)
			printf("%s\n", resultStr.c_str());
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
#if 1
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif