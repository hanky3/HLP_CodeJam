#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

int N, K;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &K);
}

int findFastPath()
{
	vector<bool> visited(100001, false);
	queue<pair<int, int>> q;
	q.push({ N, 0 });

	while (!q.empty()) {
		int pos = q.front().first;
		int step = q.front().second;
		q.pop();

		if (pos == K)
			return step;

		if (pos < 0 || pos > 100000)	
			continue;
		if (visited[pos])	
			continue;
		visited[pos] = true;

		q.push({ pos + 1, step + 1 });
		q.push({ pos - 1, step + 1 });
		q.push({ pos * 2, step + 1 });

	}
	return -1;
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
		auto ret = findFastPath();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
