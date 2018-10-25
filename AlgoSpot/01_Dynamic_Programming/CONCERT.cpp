#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_VOLUME 1001
#define MAX_SONGS	 51

FILE *fpInput;
FILE *fpOutput;

int N, volStart, volMax;
vector<int> volumeList;
int cache[MAX_SONGS][MAX_VOLUME];

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &N, &volStart, &volMax);
	memset(cache, -2, sizeof(cache));

	volumeList = vector<int>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &volumeList[index]);
	}
}

int findMaxVolume(int song, int volSum)
{
	int &ret = cache[song][volSum];

	if (ret > -2)
		return ret;

	if (song == N)
		return volSum;

	ret = -1;
	int vol = volumeList[song];

	if (volSum - vol >= 0)
		ret = max(ret, findMaxVolume(song + 1, volSum - vol));
	if (volSum + vol <= volMax)
		ret = max(ret, findMaxVolume(song + 1, volSum + vol));

	return ret;
}

void solveProblem(const char *fileName, bool isFile)
{
	fpInput = stdin;
	fpOutput = stdout;
	if (isFile) {
		fpInput = fopen(fileName, "r");
		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		int ret = findMaxVolume(0, volStart);
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif
	return 0;
}