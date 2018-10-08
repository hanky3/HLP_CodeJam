#include <stdio.h>
#include <string.h>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

FILE *fpInput;
FILE *fpOutput;


int N, M;
vector<vector<int>> cityMap;
vector<pair<int, int>> roadInfo;

class DisjointSet
{
public:
	DisjointSet(int n) : parent(n), rank(n, 1), memberSize(n, 1)
	{
		for (int i = 0; i < n; i++)
			parent[i] = i;
	}

	int find(int u) {
		if (u == parent[u])
			return u;

		parent[u] = find(parent[u]);
		return parent[u];
	}

	int getGroupSize() {
		int ret = 0;
		vector<bool> visited(parent.size(), false);
		for (int u = 0; u < parent.size(); u++) {
			int p = find(u);
			if (visited[p])
				continue;

			visited[p] = true;
			ret++;
		}
		return ret;
	}

	int merge(int u, int v) {
		if (u < 0 || v < 0)
			return max(u, v);

		u = find(u);
		v = find(v);

		if (u == v) return u;
		if (rank[u] > rank[v])
			swap(u, v);

		if (rank[u] == rank[v])
			rank[v]++;

		memberSize[u] += memberSize[v];
		parent[v] = u;

		return u;
	}

public:
	vector<int> parent, rank;
	vector<int> memberSize;
};


void readInputData()
{
	fscanf(fpInput, "%d %d\n", &N, &M);

	cityMap = vector<vector<int>>(N, vector<int>(N, 0));
	roadInfo.clear();
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < N; w++) {
			char ch;
			fscanf(fpInput, "%c", &ch);
			if (ch == 'Y') {
				cityMap[h][w] = 1;
				pair<int, int> road = { h, w };
				if (h > w)	swap(road.first, road.second);
				if (find(roadInfo.begin(), roadInfo.end(), road) == roadInfo.end())
					roadInfo.push_back(road);
			}
		}
		fscanf(fpInput, "\n");
	}
	sort(roadInfo.begin(), roadInfo.end(), [](pair<int, int> a, pair<int, int> b) {
		if (a.first == b.first) return (a.second < b.second);
		return (a.first < b.first);
	});
}


vector<int> findRoadConnected()
{
	vector<bool> roadVisited(roadInfo.size(), false);
	vector<int> roadCnt(N, 0);
	DisjointSet roadSet(N);

	int count = 0;
	for (int index = 0; index < roadInfo.size(); index++) {
		pair<int, int> road = roadInfo[index];

		if (road.first == road.second) continue;
		if (roadSet.find(road.first) == roadSet.find(road.second))
			continue;

		roadSet.merge(road.first, road.second);
		roadCnt[road.first]++;
		roadCnt[road.second]++;

		roadVisited[index] = true;
		count++;
		if (count == M) break;
	}

	if (count < M) {
		for (int index = 0; index < roadInfo.size(); index++) {
			pair<int, int> road = roadInfo[index];
			if (roadVisited[index]) continue;
			roadCnt[road.first]++;
			roadCnt[road.second]++;
			count++;
			if (count == M) break;
		}
	}

	if (count < M)
		return { -1 };

	if (roadSet.getGroupSize() > 1)
		return { -1 };

	return roadCnt;
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
		auto ret = findRoadConnected();
		for (auto v : ret) {
#ifdef _FILE_
			printf("%d ", v);
#endif
			fprintf(fpOutput, "%d ", v);
		}
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}

