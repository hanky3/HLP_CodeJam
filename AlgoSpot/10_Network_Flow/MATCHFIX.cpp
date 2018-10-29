#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
using namespace std;

const int INF = 0x0fffffff;
int MAX_V = 1001;
FILE *fpInput = NULL;
FILE *fpOutput = NULL;

int V;
vector<vector<int>> capacity;
vector<vector<int>> flow;
int networkFlow(int source, int sink)
{
	int totalFlow = 0;

	while (true)
	{
		vector<int> parent(MAX_V, -1);
		queue<int> q;
		parent[source] = source;

		q.push(source);
		while (!q.empty() && parent[sink] == -1)
		{
			int here = q.front();
			q.pop();

			for (int there = 0; there < capacity.size(); there++)
			{
				if (capacity[here][there] - flow[here][there] > 0 && parent[there] == -1)
				{
					parent[there] = here;
					q.push(there);
				}
			}
		}

		if (parent[sink] == -1) break;

		int amount = INF;
		for (int p = sink; p != source; p = parent[p])
			amount = min(capacity[parent[p]][p] - flow[parent[p]][p], amount);

		for (int p = sink; p != source; p = parent[p])
		{
			flow[parent[p]][p] += amount;
			flow[p][parent[p]] -= amount;
		}
		totalFlow += amount;
	}

	return totalFlow;
}

int numOfPlayers;
int numOfPlays;
int winPlayer;
vector<int> currentWins;
bool playerXWins(int &totalWins)
{
	int sink = numOfPlays + numOfPlayers + 1;
	for (int index = 1; index < numOfPlayers; index++)
	{
		if (currentWins[winPlayer] + flow[winPlayer][sink] <= currentWins[index] + flow[index][sink])
			return false;
	}
	totalWins = currentWins[winPlayer] + flow[winPlayer][sink];
	return true;
}

int getMaxOtherWin()
{
	int maxWins = 0;
	for (int index = 1; index < numOfPlayers; index++)
		maxWins = max(maxWins, currentWins[index]);
	return maxWins;
}

int minWinsToChamp()
{
	int totalWins;
	int source = numOfPlays + numOfPlayers;
	int sink = numOfPlays + numOfPlayers + 1;
	// 기저사례
	if (numOfPlays == 0)
	{
		if (playerXWins(totalWins))
			return totalWins;
		else
			return -1;
	}

	capacity[0][sink] = 0;
	int maxCurrentWin = *std::max_element(currentWins.begin() + 1, currentWins.end());
	int currentPlays = 0;
	for (int i = 0; i <= numOfPlays; i++)
	{
		totalWins = i + currentWins[0];

		if (totalWins <= maxCurrentWin) 
			continue;

		for (int playerIdx = 0; playerIdx < currentWins.size(); playerIdx++) 
		{
			int remainWins = (playerIdx == 0) ? (totalWins - currentWins[playerIdx]) : (totalWins - currentWins[playerIdx] - 1);
			capacity[playerIdx][sink] = remainWins;
		}

		currentPlays += networkFlow(source, sink);
		if (currentPlays == numOfPlays && playerXWins(totalWins))
			return totalWins;
	}
	return -1;
}

void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");

	if (!isFile)
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else
	{
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++)
	{
		fscanf(fpInput, "%d %d\n", &numOfPlayers, &numOfPlays);

		// 정점수는 사람+경기수+2;
		V = numOfPlayers + numOfPlays+ 2;
		flow = vector<vector<int>>(V, vector<int>(V, 0));
		capacity = vector<vector<int>>(V, vector<int>(V, 0));

		currentWins = vector<int>(numOfPlayers, 0);
		for (int j = 0; j < numOfPlayers; j++)
		{
			fscanf(fpInput, "%d", &currentWins[j]);
		}

		int source = V - 2;
		int sink = V - 1;

		// source와 play 연결
		for (int j = 0; j < numOfPlays; j++)
		{
			pair<int, int> players;
			fscanf(fpInput, "%d %d", &players.first, &players.second);
			capacity[source][j + numOfPlayers] = 1;
			capacity[j + numOfPlayers][players.first] = 1;
			capacity[j + numOfPlayers][players.second] = 1;
		}

		int ret = minWinsToChamp();
		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}