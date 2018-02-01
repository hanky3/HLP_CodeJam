#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#if 0
using namespace std;

#define MAX_BOARD_SIZE 21
#define MAX_INF 999999999
int foodCount;
int friendCount;
vector<vector<int>> foodInfo;
vector<vector<int>> friendInfo;
vector<string> friendsNameList;


FILE *fpInput = NULL;
FILE *fpOutput = NULL;

int best;
void bestSearch(vector<int> &edible, int choiceNum)
{
	if (best < choiceNum)
		return;

	int lastFriendIndex = 0;
	for (auto eatFlag : edible)
	{
		if (!eatFlag)
			break;
		lastFriendIndex++;
	}

	if (lastFriendIndex == edible.size())
	{
		if (best > choiceNum)
			best = choiceNum;
		return;
	}

	for (auto foodIndex : friendInfo[lastFriendIndex])
	{
		for (auto friendIndex : foodInfo[foodIndex])
			edible[friendIndex]++;

		bestSearch(edible, choiceNum + 1);

		for (auto friendIndex : foodInfo[foodIndex])
			edible[friendIndex]--;
	}
//	if (edible)
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
		fscanf(fpInput, "%d %d", &friendCount, &foodCount);
		foodInfo.clear();
		friendInfo.clear();

		foodInfo.resize(foodCount);
		friendInfo.resize(friendCount);
		friendsNameList.clear();

		char buf[100];
		for (int i = 0; i < friendCount; i++)
		{
			fscanf(fpInput, "%s", buf);
			friendsNameList.push_back(string(buf));
		}

		for (int i = 0; i < foodCount; i++)
		{
			int relationCount;
			fscanf(fpInput, "%d", &relationCount);

			for (int j = 0; j < relationCount; j++)
			{
				fscanf(fpInput, "%s", buf);
				auto itr = std::find(friendsNameList.begin(), friendsNameList.end(), string(buf));

				int index = itr - friendsNameList.begin();
				friendInfo[index].push_back(i);
				foodInfo[i].push_back(index);
			}
			sort(foodInfo[i].begin(), foodInfo[i].end());
		}

		best = MAX_INF;
		vector<int> edible(friendCount, 0);
		bestSearch(edible, 0);
		fprintf(fpOutput, "%d\n", best);
		if (isFile)
			printf("%d\n", best);
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