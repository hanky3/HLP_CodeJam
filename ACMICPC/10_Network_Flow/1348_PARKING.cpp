#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <map>

using namespace std;

FILE *fpInput;
FILE *fpOutput;
#define INF	987654321

int R, C;
vector<vector<pair<int, int>>> adj;
vector<vector<char>> mapInfo;
map<pair<int, int>, int> carNumber, parkingNumber;
const pair<int, int> directions[] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

void readInputData()
{
	fscanf(fpInput, "%d %d", &R, &C);

	carNumber.clear();	parkingNumber.clear();

	mapInfo = vector<vector<char>>(R, vector<char>(C));
	int car = 0, park = 0;
	for (int h = 0; h < R; h++) {
		char buf[51];
		fscanf(fpInput, "%s", buf);
		for (int w = 0; w < C; w++) {
			mapInfo[h][w] = buf[w];
			if (mapInfo[h][w] == 'C')
				carNumber[{ h, w }] = car++;
			if (mapInfo[h][w] == 'P')
				parkingNumber[{ h, w }] = park++;
		}
	}

}
vector<int> aMatch;
vector<int> bMatch;
bool dfs(int a, vector<bool> &visited, int distance)
{
	if (visited[a])
		return false;

	visited[a] = true;
	for (auto there : adj[a]) {
		int b = there.first;
		int thereDist = there.second;
		if (thereDist <= distance) {
			if (bMatch[b] == -1 || dfs(bMatch[b], visited, distance)) {
				aMatch[a] = b;
				bMatch[b] = a;
				return true;
			}
		}
	}
	return false;
}

bool bipartiteMatch(int aCount, int bCount, int k)
{
	aMatch = vector<int>(aCount, -1);
	bMatch = vector<int>(bCount, -1);

	int size = 0;
	for (int start = 0; start < aCount; start++) {
		vector<bool> visited(aCount, false);
		if (dfs(start, visited, k))
			size++;
	}
	return (size==aCount);
}

void makeParkingPath(pair<int, int> carPos)
{
	vector<vector<int>> distance(R, vector<int>(C, -1));
	queue <pair<pair<int, int>, int>> q;
	q.push({ carPos, 0 });
	distance[carPos.first][carPos.second] = 0;
	while (!q.empty()) {
		auto currPos = q.front().first;
		auto dist = q.front().second;
		q.pop();
		if (mapInfo[currPos.first][currPos.second] == 'P') {
			adj[carNumber[carPos]].push_back({ parkingNumber[currPos], dist });
		}

		for (auto dir : directions) {
			pair<int, int> nextPos(currPos.first + dir.first, currPos.second + dir.second);
			int nextDist = dist + 1;
			if (nextPos.first < 0 || nextPos.first >= R || nextPos.second < 0 || nextPos.second >= C)
				continue;
			if (distance[nextPos.first][nextPos.second] == -1 && mapInfo[nextPos.first][nextPos.second] != 'X') {
				distance[nextPos.first][nextPos.second] = nextDist;
				q.push({ nextPos, nextDist });
			}
		}
	}
}

void makeGraph()
{
	adj = vector<vector<pair<int, int>>>(carNumber.size() + parkingNumber.size());
	for (auto carPos : carNumber) {
		makeParkingPath(carPos.first);
	}
}

int parkingTime()
{
	if (carNumber.size() == 0)
		return 0;

	int lo = 0, hi = 5000;
	while (lo + 1 < hi) {
		int mid = (lo + hi) / 2;
		if (bipartiteMatch(carNumber.size(), parkingNumber.size(), mid))
			hi = mid;
		else
			lo = mid;
	}
	if (bipartiteMatch(carNumber.size(), parkingNumber.size(), hi))
		return hi;
	return -1;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#if _FILE_
	fpInput = fopen(argv[1], "r");
	fpOutput = fopen("output.txt", "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase > 0) {
		readInputData();
		makeGraph();
		auto ret = parkingTime();
#if _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}