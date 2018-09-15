#include <stdio.h>
#include <string.h>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <queue>

using namespace std;

FILE *fpInput;
FILE *fpOutput;


int N, M, V;
vector<vector<int>> adj;

void readInputData()
{
	fscanf(fpInput, "%d %d %d\n", &N, &M, &V);
	adj = vector<vector<int>>(N);

	for (int n = 0; n < M; n++) {
		int in, out;
		fscanf(fpInput, "%d %d\n", &in, &out);
		in--; out--;

		auto itr = find(adj[in].begin(), adj[in].end(), out);
		if (itr == adj[in].end())
			adj[in].push_back(out);
	
		itr = find(adj[out].begin(), adj[out].end(), in);
		if (itr == adj[out].end())
			adj[out].push_back(in);
	}

	for (int i = 0; i < N; i++) {
		sort(adj[i].begin(), adj[i].end());
	}
}

vector<int> calcBFS(int start)
{
	vector<int> path;

	vector<bool> visited(N, false);
	queue<int> q;
	q.push(start);

	while (!q.empty()) {
		int here = q.front();
		q.pop();

		if (visited[here]) continue;
		path.push_back(here);
		visited[here] = true;

		for (auto there : adj[here]) {
			if (visited[there]) continue;
			q.push(there);
		}
	}

	return path;
}

void calcDFS(int here, vector<int> &path, vector<bool> &visited)
{
	if (visited[here])
		return;

	path.push_back(here);
	visited[here] = true;

	for (auto there : adj[here]) {
		if (visited[there]) continue;
		calcDFS(there, path, visited);
	}
}

void  printPath(vector<int> &path)
{
	for (auto p : path) {
#ifdef _FILE_
		printf("%d ", p + 1);
#endif
		fprintf(fpOutput, "%d ", p + 1);
	}
#ifdef _FILE_
	printf("\n");
#endif
	fprintf(fpOutput, "\n");
}

void generatePath()
{
	vector<bool> visited(N, false);
	vector<int> path;

	calcDFS(V - 1, path, visited);
	printPath(path);

	path = calcBFS(V - 1);
	printPath(path);
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
	while (testCase > 0) {
		readInputData();
		generatePath();
		testCase--;
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}
