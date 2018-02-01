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

#if 0
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
#define MAX_STUDENT_CNT			10

int studentCount;
int studentPairCount;
vector<int> studentPairList[MAX_STUDENT_CNT];

void readStudentPairInfo()
{
	for (int index = 0; index < MAX_STUDENT_CNT; index++)
		studentPairList[index].clear();

	for (int index = 0; index < studentPairCount; index++)
	{
		pair<int, int> studentPair;
		fscanf(fpInput, "%d %d", &studentPair.first, &studentPair.second);
		
		if (studentPair.first > studentPair.second)
			swap(studentPair.first, studentPair.second);

		studentPairList[studentPair.first].push_back(studentPair.second);
	}

	for (int index = 0; index < studentCount; index++)
		sort(studentPairList[index].begin(), studentPairList[index].end());
}

int friendBitMap;
void setFriendBit(int &bitmap, int friendNum)
{
	bitmap |= (1 << friendNum);
}

void disbleFriendBit(int &bitmap, int friendNum)
{
	bitmap &= ~(1 << friendNum);
}

bool isFriendBitSet(int &bitmap, int friendNum)
{
	return (bitmap & (1 << friendNum)) > 0;
}

bool allFriendBitSet(int &bitmap, int friendCnt, int &next)
{
	for (int index = 0; index < friendCnt; index++)
	{
		if (!isFriendBitSet(bitmap, index))
		{
			next = index;
			return false;
		}
	}
	return true;
}

int friendPairCount(int &selectedFriendBitmap)
{
	int nextStudent = 0;
	if (allFriendBitSet(selectedFriendBitmap, studentCount, nextStudent))
		return 1;

	int ret = 0;
	setFriendBit(selectedFriendBitmap, nextStudent);
	for (int index = 0; index < studentPairList[nextStudent].size(); index++)
	{
		int friendPair = studentPairList[nextStudent][index];

		if (isFriendBitSet(selectedFriendBitmap, friendPair))
			continue;

		setFriendBit(selectedFriendBitmap, friendPair);
		ret += friendPairCount(selectedFriendBitmap);
		disbleFriendBit(selectedFriendBitmap, friendPair);
	}
	disbleFriendBit(selectedFriendBitmap, nextStudent);

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
		fscanf(fpInput, "%d %d", &studentCount, &studentPairCount);
		readStudentPairInfo();
		int selected = 0;
		int ret = friendPairCount(selected);

		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);

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
#endif