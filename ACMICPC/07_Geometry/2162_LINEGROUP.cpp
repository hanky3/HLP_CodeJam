#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
#define MAX_TEXT_SIZE				1000000
#define MAX_KEYWORD_SIZE	36

FILE *fpInput;
FILE *fpOutput;

struct LineInfo {
	pair<int, int> startPos;
	pair<int, int> endPos;
};

int N;
vector<LineInfo> lines;

void readInputData()
{
	fscanf(fpInput, "%d", &N);

	lines = vector<LineInfo>(N);
	for (int i = 0; i < N; i++) {
		LineInfo line;
		fscanf(fpInput, "%d %d %d %d", &line.startPos.first, &line.startPos.second, &line.endPos.first, &line.endPos.second);
		if (line.startPos.first > line.endPos.first)
			swap(line.startPos, line.endPos);
		lines[i] = line;
	}
}

double ccw(pair<int, int> a, pair<int, int> b, pair<int, int> c)
{
	double op = a.first*b.second + b.first*c.second + c.first*a.second;
	op -= (a.second*b.first + b.second*c.first + c.second*a.first);
	return op;
}

int isIntersectLine(LineInfo x, LineInfo y) {
	double ab = ccw(x.startPos, x.endPos, y.startPos)*ccw(x.startPos, x.endPos, y.endPos);
	double cd = ccw(y.startPos, y.endPos, x.startPos)*ccw(y.startPos, y.endPos, x.endPos);
	if (ab == 0 && cd == 0) {
		if (x.startPos > x.endPos)
			swap(x.startPos, x.endPos);
		if (y.startPos > y.endPos)
			swap(y.startPos, y.endPos);
		return x.startPos <= y.endPos && y.startPos <= x.endPos;
	}
	return ab <= 0 && cd <= 0;
}

int dfs(int here, vector<bool> &visited)
{
	int traced = 1;
	visited[here] = true;

	for (int there = 0; there < N; there++) {
		if (visited[there]) continue;
		if (isIntersectLine(lines[here], lines[there])) {
			traced += dfs(there, visited);
		}
	}
	return traced;
}

pair<int, int> findLineGroup()
{
	int groupCount = 0, maxLines = 0;
	vector<bool> visited(N, 0);
	for (int here = 0; here < N; here++) {
		if (visited[here]) continue;
		int lineCount = dfs(here, visited);
		maxLines = max(maxLines, lineCount);
		groupCount++;
	}

	return { groupCount, maxLines };
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
		auto ret = findLineGroup();
#ifdef _FILE_
		printf("%d\n%d\n", ret.first, ret.second);
#endif
		fprintf(fpOutput, "%d\n%d\n", ret.first, ret.second);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
