#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_SUBSTR_COUNT	(15)
#define MAX_STR_LENGTH		(40)
#define MAX_STR_SIZE	((MAX_SUBSTR_COUNT)*(MAX_STR_LENGTH)+(1))

pair<int, int> cache[1 << MAX_SUBSTR_COUNT][MAX_SUBSTR_COUNT + 1];
int overlapCache[MAX_SUBSTR_COUNT+1][MAX_SUBSTR_COUNT+1];
vector<string> subStrings;

void readInputData()
{	
	// initilization
	memset(cache, -1, sizeof(cache));
	memset(overlapCache, -1, sizeof(overlapCache));
	subStrings.clear();

	int subStrCount;
	fscanf(fpInput, "%d", &subStrCount);

	char buf[MAX_STR_LENGTH+1];
	for (int index = 0; index < subStrCount; index++)
	{
		fscanf(fpInput, "%s", buf);
		subStrings.push_back(buf);
	}

	//path = vector<int>(subStrCount, -1);
}

int maxOverlap(int first, int second)
{
	if (first < 0 || second < 0)
		return 0;

	int &ret = overlapCache[first][second];

	if (ret != -1)
		return ret;

	ret = 0;
	string firstStr = subStrings[first];
	string secondStr = subStrings[second];

	for (int start = 0; start < firstStr.size(); start++)
	{
		int overlap = min((firstStr.size() - start), secondStr.size());
		string bSubStr = secondStr.substr(0, overlap);
		if (firstStr.substr(start).find(bSubStr) != string::npos)
		{
			ret = overlap;
			break;
		}
	}
	return ret;
}


pair<int, int> restore(int selected, int prev)
{
	if (selected == ((1 << subStrings.size()) - 1)) {
		return make_pair(0, -1);
	}

	pair<int, int> &ret = cache[selected][prev];
	if (ret.first != -1)
		return ret;

	string prevStr = "";
	if (prev > 0)
		prevStr.append(subStrings[prev-1]);

	ret.first = MAX_STR_SIZE;
	for (int index = 0; index < subStrings.size(); index++)
	{
		int pos = (1 << index);
		if (selected & pos)
			continue;
		string currStr = subStrings[index];

		int mergeLength = 0;
		mergeLength += currStr.size();
		mergeLength -= maxOverlap(prev-1, index);

		int curr = index;
		if (maxOverlap(prev - 1, index) == currStr.size())
			curr = prev - 1;
		pair<int, int> nextOverlap = restore(selected | pos, curr + 1);
		mergeLength += nextOverlap.first;

		if (mergeLength < ret.first) {
			ret.first = mergeLength;
			ret.second = index;
		}

	}

	return ret;
}

string reconstruct(int start, int selected)
{
	pair<int, int> next = restore(selected, start);

	if (next.second < 0)
		return "";

	int overlap = maxOverlap(start-1, next.second);
	int nextStart = next.second+1;
	if (overlap == subStrings[next.second].size())
		nextStart = start;

	string ret = subStrings[next.second].substr(overlap);
	ret += reconstruct(nextStart, selected | (1 << next.second));

	return ret;
}

void solveProblem(const char *fileName, bool isFile)
{
	if (isFile)
	{
		fpInput = fopen(fileName, "r");

		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}
	else
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	int testCase = 0;

	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();

		pair<int, int> ret = restore(0, 0);
		string result = subStrings[ret.second];
		result = reconstruct(0, 0);

		if (isFile)
			printf("%s\n", result.c_str());
		fprintf(fpOutput, "%s\n", result.c_str());

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}
