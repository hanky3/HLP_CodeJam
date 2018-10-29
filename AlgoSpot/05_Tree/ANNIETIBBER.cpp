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

const double PI = 2.0 * acos(0.0);

struct FenwickTree {
	vector<int> trees;

	FenwickTree(int n) : trees(n + 1, 0) {};
	void add(int pos, int val) {
		while (pos < trees.size()) {
			trees[pos] += val;
			pos += (pos & -pos);
		}
	}

	int sum(int pos) {
		int ret = 0;
		while (pos > 0) {
			ret += trees[pos];
			pos &= (pos - 1);
		}
		return ret;
	}
};

int pointCnt, p, q;
vector<pair<int, int>> starPoints;
vector<pair<double, int>> starPointByP;
vector<pair<double, int>> starPointByQ;
vector<int> starPointsPIndex;
vector<int> starPointsQIndex;

bool compare(pair<double, int> &a, pair<double, int> &b) {
	return (a.first > b.first);
}

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &pointCnt, &p, &q);

	if (p > q)
		swap(p, q);

	starPoints = vector<pair<int, int>>(pointCnt);
	starPointByP = vector<pair<double, int>>(pointCnt);
	starPointByQ = vector<pair<double, int>>(pointCnt);
	starPointsPIndex = vector<int>(pointCnt, 0);
	starPointsQIndex = vector<int>(pointCnt, 0);

	for (int index = 0; index < pointCnt; index++) {
		fscanf(fpInput, "%d %d", &starPoints[index].first, &starPoints[index].second);
		
		pair<double, int> starAngleP;
		starAngleP.first = fmod(atan2(starPoints[index].second, starPoints[index].first - p) + 2 * PI, 2 * PI);
		starAngleP.second = index;
		starPointByP[index] = starAngleP;

		pair<double, int> starAngleQ;
		starAngleQ.first = fmod(atan2(starPoints[index].second, starPoints[index].first - q) + 2 * PI, 2 * PI);
		starAngleQ.second = index;
		starPointByQ[index] = starAngleQ;
	}
	sort(starPointByP.begin(), starPointByP.end(), compare);
	sort(starPointByQ.begin(), starPointByQ.end(), compare);

	for (int index = 0; index < pointCnt; index++) {
		starPointsQIndex[starPointByQ[index].second] = index;
	}
}


long long findAsymmetricLines()
{
	long long ret = 0;
	FenwickTree qUsed(starPointsQIndex.size());

	for (auto star : starPointByP) {
		int qIndex = starPointsQIndex[star.second];
		ret += qIndex - qUsed.sum(qIndex);
		qUsed.add(qIndex + 1, 1);
	}


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
		long long ret = findAsymmetricLines();
		if (isFile)
			printf("%lld\n", ret);
		fprintf(fpOutput, "%lld\n", ret);

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