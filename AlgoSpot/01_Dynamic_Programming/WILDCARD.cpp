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

#define MAX_STR_LENGTH		(101)

int cache[MAX_STR_LENGTH][MAX_STR_LENGTH];
string wildcardStr;
vector<string> queryStrings;


void readInputData()
{
	// initilization
	char buf[MAX_STR_LENGTH];
	memset(cache, -1, sizeof(cache));
	queryStrings.clear();

	int queryCnt;
	fscanf(fpInput, "%s", buf);
	wildcardStr = buf;

	queryStrings.clear();
	fscanf(fpInput, "%d", &queryCnt);
	for (int index = 0; index < queryCnt; index++)
	{
		fscanf(fpInput, "%s", buf);
		queryStrings.push_back(buf);
	}
}

int wordMatching(int pos1, int pos2, string query)
{
	if (pos1 == wildcardStr.length() &&
		pos2 == query.length())
		return true;

	if (pos1 >= wildcardStr.length())
		return false;

	if (pos2 >= query.length()) {
		if (wildcardStr.find('*', pos1) == string::npos)
			return false;
	}


	int &ret = cache[pos1][pos2];

	if (ret != -1)
		return ret;

	ret = false;
	if (wildcardStr[pos1] != '?' && wildcardStr[pos1] != '*') {
		if (wildcardStr[pos1] != query[pos2])
			return ret;
	}

	if (wildcardStr[pos1] == '*') {
		for (int nextPos = pos2; nextPos <= query.length(); nextPos++) {
			if (wordMatching(pos1 + 1, nextPos, query)) {
				ret = true;
				break;
			}
		}
		return ret;
	}

	ret = wordMatching(pos1 + 1, pos2 + 1, query);
	return ret;
}

vector<string> findMatchedStr()
{
	vector<string> ret;
	for (auto query : queryStrings) {
		memset(cache, -1, sizeof(cache));
		if (!wordMatching(0, 0, query))
			continue;

		ret.push_back(query);
	}

	sort(ret.begin(), ret.end());
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

		vector<string> ret = findMatchedStr();
		for (auto query : ret) {
			if (isFile)
				printf("%s\n", query.c_str());
			fprintf(fpOutput, "%s\n", query.c_str());
		}

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