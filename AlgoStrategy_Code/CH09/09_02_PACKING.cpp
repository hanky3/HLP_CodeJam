#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#if 0
using namespace std;

#define MAX_OBJECT 101
#define MAX_WEIGHT 1001
int cache[MAX_OBJECT][MAX_WEIGHT];
unsigned long long cacheObjs[MAX_OBJECT][MAX_WEIGHT][2];

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

vector<pair<int, int>> travelObjList;
vector<string> travelObjNames;


int getMaxPackingPreference(int start, int remainWeight, unsigned long long selectedObjs[2])
{
	int &ret = cache[start][remainWeight];
	if (ret >= 0)
	{
		memcpy(selectedObjs, cacheObjs[start][remainWeight], 2 * sizeof(unsigned long long));
		return ret;
	}

	memset(selectedObjs, 0, 2*sizeof(unsigned long long));
	if (start == travelObjList.size())
		return 0;

	ret = 0;
	int preferenceA, preferenceB;
	unsigned long long objA[2], objB[2];

	memset(objA, 0, sizeof(unsigned long long) * 2);
	memset(objB, 0, sizeof(unsigned long long) * 2);

	preferenceA = -1;
	if (travelObjList[start].first <= remainWeight)
	{
		preferenceA = travelObjList[start].second;
		preferenceA += getMaxPackingPreference(start + 1, remainWeight - travelObjList[start].first, objA);
	}
	preferenceB = getMaxPackingPreference(start + 1, remainWeight, objB);

	if (preferenceA > preferenceB)
	{
		ret = preferenceA;
		memcpy(selectedObjs, objA, sizeof(unsigned long long) * 2);
		selectedObjs[start / 64] |= (1 << (start % 64));
	}
	else
	{
		memcpy(selectedObjs, objB, sizeof(unsigned long long) * 2);
		ret = preferenceB;
	}

	for (int i = 0; i < 2; i++)
		cacheObjs[start][remainWeight][i] = selectedObjs[i];
	return ret;
}

#if 0
int getMaxPackingPreference(int start, int remainWeight)
{
	int &ret = cache[start][remainWeight];
	if (ret >= 0)
		return ret;

	bool isEnough = false;
	for (int index = start; index < travelObjList.size(); index++)
	{
		if (travelObjList[index].first <= remainWeight)
		{
			isEnough = true;
			break;
		}
	}

	if (!isEnough)
	{
		ret = 0;
		return ret;
	}

	if (start == travelObjList.size() - 1)
		return travelObjList[start].second;

	ret = 0;
	int preferenceA, preferenceB;
	preferenceA = 0;
	if (travelObjList[start].first <= remainWeight)
	{
		preferenceA = travelObjList[start].second;
		preferenceA += getMaxPackingPreference(start + 1, remainWeight - travelObjList[start].first);
	}
	preferenceB = getMaxPackingPreference(start + 1, remainWeight);

	if (preferenceA > preferenceB)
	{
		ret = preferenceA;
	}
	else
	{
		ret = preferenceB;
	}
	return ret;
}


void reconstruct(int remainWeight, int item, vector<string>& picked)
{
	if (item == travelObjList.size())
		return;

	if (getMaxPackingPreference(item, remainWeight) == getMaxPackingPreference(item+1, remainWeight)) {
		reconstruct(remainWeight, item + 1, picked);
	}
	else
	{
		picked.push_back(travelObjNames[item]);
		reconstruct(remainWeight - travelObjList[item].first, item + 1, picked);
	}
}
#endif

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

	char buf[21];
	for (int i = 0; i < testcase; i++)
	{
		int objCount, maxWeight;
		fscanf(fpInput, "%d %d", &objCount, &maxWeight);

		memset(cache, 0xFFFFFFFF, MAX_OBJECT*MAX_WEIGHT*sizeof(int));
		memset(cacheObjs, 0x00, MAX_OBJECT*MAX_WEIGHT*sizeof(long long)* 2);

		travelObjList.clear();
		travelObjList = vector<pair<int, int>>(objCount, make_pair(0, 0));
		travelObjNames.clear();
		travelObjNames = vector<string>(objCount);

		for (int i = 0; i < objCount; i++)
		{
			fscanf(fpInput, "%s %d %d", buf, &travelObjList[i].first, &travelObjList[i].second);
			travelObjNames[i] = buf;
		}

		unsigned long long objBitList[2];
		int result = getMaxPackingPreference(0, maxWeight, objBitList);
		vector<string> objNames;

		for (int i = 0; i < objCount; i++)
		{
			if (objBitList[i / 64] & (1 << i))
			{
				objNames.push_back(travelObjNames[i]);
			}
		}
		//reconstruct(maxWeight, 0, objNames);

		fprintf(fpOutput, "%d %d\n", result, objNames.size());
		if (isFile)
			printf("%d %d\n", result, objNames.size());

		for (auto objName : objNames)
		{
			fprintf(fpOutput, "%s\n", objName.c_str());
			if (isFile)
				printf("%s\n", objName.c_str());
		}

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