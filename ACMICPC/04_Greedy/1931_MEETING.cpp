#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

#define MAX_NUMBER_SIZE	100001

FILE *fpInput;
FILE *fpOutput;

int N;
vector<pair<int, int>> meetingTimes;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	meetingTimes = vector<pair<int, int>>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%d %d", &meetingTimes[i].first, &meetingTimes[i].second);
	}
	sort(meetingTimes.begin(), meetingTimes.end(), [&](pair<int, int> &a, pair<int, int> &b) {
		if (a.second == b.second) 
			return (a.first < b.first);
		return (a.second < b.second);
	});
}

int maxMeetings()
{
	int ret = 0;

	int endTime = -1;
	for (auto m : meetingTimes) {
		if (m.first < endTime)
			continue;

		endTime = m.second;
		ret++;
	}

	return ret;
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
		auto ret = maxMeetings();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
