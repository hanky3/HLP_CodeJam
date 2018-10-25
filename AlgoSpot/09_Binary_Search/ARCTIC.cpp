#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N;
vector<pair<double, double>> positions;
vector<vector<int>> adjPos;

void readInputData()
{
	fscanf(fpInput, "%d", &N);

	adjPos = vector<vector<int>>(N);
	positions = vector<pair<double, double>>(N);
	for (int i = 0; i < N; i++) {
		pair<double, double> pos;
		fscanf(fpInput, "%lf %lf", &positions[i].first, &positions[i].second);
	}
	sort(positions.begin(), positions.end());
}

double getDistance(pair<double, double> a, pair<double, double> b)
{
	return sqrt((a.first - b.first)*(a.first - b.first) + (a.second - b.second)*(a.second - b.second));
}

void makeGraphs(double distance)
{
	adjPos = vector<vector<int>>(N);
	for (int curr = 0; curr < positions.size(); curr++) { 
		for (int next = curr + 1; next < positions.size(); next++) {
			double xDist = abs(positions[curr].first - positions[next].first);
			if (xDist > distance)
				break;
			if (getDistance(positions[curr], positions[next]) <= distance) {
				adjPos[curr].push_back(next);
				adjPos[next].push_back(curr);
			}
		}
	}
}

void dfs(int here, vector<bool> &visited)
{
	visited[here] = true;
	for (auto there : adjPos[here]) {
		if (!visited[there])
			dfs(there, visited);
	}
}

bool isAllConnected(double dist)
{
	vector<bool> visited(N, false);
	makeGraphs(dist);
	dfs(0, visited);
	for (auto v : visited) {
		if (!v) return false;
	}
	return true;
}

double minPower()
{
	double lo, hi, mid;
	lo = 0;
	hi = 1000000;
	for (int iter = 0; iter < 100; iter++) {
		mid = (lo + hi) / 2;
		if (isAllConnected(mid))
			hi = mid;
		else
			lo = mid;
	}
	return mid;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	string outputFileName = string(argv[1]);
	outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
	fpOutput = fopen(outputFileName.c_str(), "w");
#endif

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = minPower();
#ifdef _FILE_
		printf("%.2f\n", ret);
#endif
		fprintf(fpOutput, "%.2f\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}