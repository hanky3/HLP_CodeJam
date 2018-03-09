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
#define MAX_GRID_WIDTH	(1000000000)
#define MAX_GRID_HEIGHT	(1000000000)

vector<pair<int, int>> infectedLocations;
vector<bool> infectedAdjFlagList;

const pair<int, int> direction[4] = {
	make_pair(1, 0),
	make_pair(-1, 0),
	make_pair(0, 1),
	make_pair(0, -1),
};

int findInfestinationTime()
{
	vector<pair<int, int>> candidateLoc;
	vector<pair<int, int>> newInfectedLoc;
	vector<pair<int, int>> updatedInfectedLoc;
	int infectionTime = 0;

	updatedInfectedLoc = infectedLocations;
	while (true)
	{
		//candidateLoc.clear();
		newInfectedLoc.clear();

		int locIndex = 0;
		for (auto loc : updatedInfectedLoc)
		{
			bool adjLocInfected = true;
			for (int index = 0; index < 4; index++)
			{
				pair<int, int> adjLoc = make_pair(loc.first + direction[index].first, loc.second + direction[index].second);
				if (adjLoc.first <= 0 || adjLoc.second <= 0
					|| adjLoc.first > MAX_GRID_HEIGHT || adjLoc.second > MAX_GRID_WIDTH)
					continue;

				auto itrInfected = std::find(infectedLocations.begin(), infectedLocations.end(), adjLoc);
				if (itrInfected != infectedLocations.end())
					continue;

				adjLocInfected = false;

				auto itrNewInfected = std::find(newInfectedLoc.begin(), newInfectedLoc.end(), adjLoc);
				if (itrNewInfected != newInfectedLoc.end())
					continue;

				auto itrCandidate = std::find(candidateLoc.begin(), candidateLoc.end(), adjLoc);
				if (itrCandidate == candidateLoc.end())
					candidateLoc.push_back(adjLoc);
				else
				{
					newInfectedLoc.push_back(adjLoc);
					candidateLoc.erase(itrCandidate);
				}
			}
			//infectedAdjFlagList[locIndex] = adjLocInfected;
			locIndex++;
		}

		if (newInfectedLoc.size() == 0)
			break;

		for (auto loc : newInfectedLoc)
		{
			infectedLocations.push_back(loc);
		}

		sort(infectedLocations.begin(), infectedLocations.end());
		updatedInfectedLoc = newInfectedLoc;

		infectionTime++;
	}

	return infectionTime;
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
		int locationCnt = 0;
		fscanf(fpInput, "%d", &locationCnt);

		infectedLocations = vector<pair<int, int>>(locationCnt);
		infectedAdjFlagList = vector<bool>(locationCnt);

		for (int index = 0; index < locationCnt; index++)
		{
			fscanf(fpInput, "%d %d", &infectedLocations[index].first, &infectedLocations[index].second);
			infectedAdjFlagList[index] = false;
		}

		int ret = findInfestinationTime();
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
#if 1
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif