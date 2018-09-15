#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>


using namespace std;

#define MAX_TEXT_SIZE	10

FILE *fpInput;
FILE *fpOutput;

int N;
vector<string> colorStrInfo;
map<string, pair<int, int>> registanceInfo;

void createRegistanceMap()
{
	registanceInfo.clear();
	registanceInfo["black"] = { 0, 1 };
	registanceInfo["brown"] = { 1, 10 };
	registanceInfo["red"] = { 2, 100 };
	registanceInfo["orange"] = { 3, 1000 };
	registanceInfo["yellow"] = { 4, 10000 };
	registanceInfo["green"] = { 5, 100000 };
	registanceInfo["blue"] = { 6, 1000000 };
	registanceInfo["violet"] = { 7, 10000000 };
	registanceInfo["grey"] = { 8, 100000000 };
	registanceInfo["white"] = { 9, 1000000000 };
}

void readInputData()
{
	char buf[MAX_TEXT_SIZE];
	colorStrInfo.clear();
	for (int i = 0; i < 3; i++) {
		fscanf(fpInput, "%s", buf);
		colorStrInfo.push_back(buf);
	}
	createRegistanceMap();
}

long long calcRegistance()
{
	long long val;
	val = registanceInfo[colorStrInfo[0]].first;
	val *= 10;
	val += registanceInfo[colorStrInfo[1]].first;
	val *= registanceInfo[colorStrInfo[2]].second;
	return val;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	fpOutput = fopen("output.txt", "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = calcRegistance();
#ifdef _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
