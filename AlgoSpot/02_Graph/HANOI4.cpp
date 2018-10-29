#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <map>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_STATE_BITS ((1 << 24))

int ringCnt;
unsigned int startState;
unsigned int endState;

int get2BitValue(unsigned int mask, int pos) {
	int shift = pos * 2;
	return (mask >> shift) & 0x03;
}

void set2BitValue(unsigned int &mask, int pos, int val) {
	int shift = pos * 2;
	mask &= ~(0x03u << shift);
	mask |= (val << shift);
}

void readInputData()
{
	fscanf(fpInput, "%d", &ringCnt);

	//HanoiState hanoiState(ringCnt);
	startState = 0;
	for (int pos = 0; pos < 4; pos++) {
		int count;
		fscanf(fpInput, "%d", &count);

		while (count > 0) {
			int ringNum;
			fscanf(fpInput, "%d", &ringNum);
			set2BitValue(startState, ringNum - 1, pos);
			//hanoiState.add(pos, ringNum - 1);
			count--;
		}
	}

	endState = 0;
	for (int ring = ringCnt - 1; ring >= 0; ring--) {
		set2BitValue(endState, ring, 3);
	}
}

int sgn(int x) {
	if (!x) return 0;
	return x > 0 ? 1 : -1;
}

int incr(int x) {
	if (x < 0)	return x - 1;
	return x + 1;
}

int c[MAX_STATE_BITS];
int minHanoiMove()
{
	if (startState == endState)
		return 0;

	queue<unsigned int> q;
	memset(c, 0, sizeof(c));

	q.push(startState);
	q.push(endState);
	c[startState] = 1;
	c[endState] = -1;

	while (!q.empty()) {
		unsigned int here = q.front();
		q.pop();

		int top[4] = { -1, -1, -1, -1 };
		for (int ring = ringCnt-1; ring >= 0; ring--) {
			top[get2BitValue(here, ring)] = ring;
		}

		for (int prev = 0; prev < 4; prev++) {
			if (top[prev] < 0)
				continue;

			for (int next = 0; next < 4; next++) {
				if (prev == next || 
					((top[next] != -1) && (top[prev] > top[next])))
					continue;

				unsigned int nextState = here;
				set2BitValue(nextState, top[prev], next);

				if (c[nextState] == 0) {
					q.push(nextState);
					c[nextState] = incr(c[here]);
				}
				else if (sgn(c[nextState]) != sgn(c[here])) {
					return abs(c[nextState]) + abs(c[here]) - 1;
				}
			}
		}
	}

	return -1;
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

		int ret = minHanoiMove();
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