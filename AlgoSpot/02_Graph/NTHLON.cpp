#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <functional>

using namespace std;

#define MAX_ELAPSE_TIME	(200)

FILE *fpInput;
FILE *fpOutput;

int V, E, n, m;
//vector<vector<pair<int, int>>> adj;
vector<pair<int, int>> elapseTimes;

void readInputData()
{
	int courseCnt;
	fscanf(fpInput, "%d", &courseCnt);

	elapseTimes = vector<pair<int, int>>(courseCnt, pair<int, int>());
	for (int i = 0; i < courseCnt; i++) {
		pair<int, int> timeInfo;
		fscanf(fpInput, "%d %d", &timeInfo.first, &timeInfo.second);
		elapseTimes[i] = timeInfo;
	}
}

typedef pair<int, pair<int, int>> TIME_INFO;
int minFinishTime()
{
	vector<TIME_INFO> finishTimes;
	priority_queue <pair<TIME_INFO, int>, vector<pair<TIME_INFO, int>>,
		std::greater<pair<TIME_INFO, int>>> q;

	// 시간별 노드 생성
	int src = (MAX_ELAPSE_TIME * 2) + 1;
	finishTimes = vector<TIME_INFO>(src+1, TIME_INFO(numeric_limits<int>::max(), pair<int, int>()));

	finishTimes[src] = make_pair(0, make_pair(0, 0));
	q.push(make_pair(finishTimes[src], src));

	while (!q.empty()) {
		int here = q.top().second;
		TIME_INFO cost = q.top().first;
		q.pop();

		if (finishTimes[here].first < cost.first)
			continue;

		for (auto timeInfo : elapseTimes) {
			int prevTimeDiff = 0;
			int timeDiff = (timeInfo.first - timeInfo.second);
			if (here != src)
				prevTimeDiff = here - MAX_ELAPSE_TIME;

			int nextTimeDiff = prevTimeDiff + (timeInfo.first - timeInfo.second);
			if (nextTimeDiff > MAX_ELAPSE_TIME || nextTimeDiff < -MAX_ELAPSE_TIME)
				continue;

			TIME_INFO nextCost(cost.first + (timeInfo.first + timeInfo.second), 
									make_pair(cost.second.first + timeInfo.first, cost.second.second + timeInfo.second));
			int there = nextTimeDiff + MAX_ELAPSE_TIME;
			if (nextCost.first < finishTimes[there].first) {
				finishTimes[there] = nextCost;
				q.push(make_pair(nextCost, there));
			}
		}
	}

	int ret = -1;
	if (finishTimes[MAX_ELAPSE_TIME].first != numeric_limits<int>::max())
		ret = (finishTimes[MAX_ELAPSE_TIME].first/2);
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

		int ret = minFinishTime();
		if (isFile) {
			if (ret < 0)
				printf("IMPOSSIBLE\n", ret);
			else
				printf("%d\n", ret);
		}

		if (ret < 0)	
			fprintf(fpOutput, "IMPOSSIBLE\n");
		else
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