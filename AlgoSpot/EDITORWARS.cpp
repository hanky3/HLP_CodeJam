#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <time.h>
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

const char* comments[] = { "ACK", "DIS", };

//---------------------------------------------------------------------------------------------
// *** 상호배타적 집합 구현
//---------------------------------------------------------------------------------------------
class DisjointSet
{
public:
	DisjointSet(int n) : parent(n), rank(n, 1), memberSize(n, 1), enemy(n, -1)
	{
		for (int i = 0; i < n; i++)
		{
			parent[i] = i;
		}
	}

	int find(int u)
	{
		if (u == parent[u])
			return u;

		parent[u] = find(parent[u]);
		return parent[u];
	}

	int getSize(int u)
	{
		return memberSize[u];
	}

	int getEnemy(int u)
	{
		return enemy[u];
	}

	int merge(int u, int v)
	{
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

	bool ack(int u, int v)
	{
		u = find(u);
		v = find(v);

		if (enemy[u] == v || enemy[v] == u)
			return false;

		int mergeU = merge(u, v);
		int enemy_u = enemy[u];
		int enemy_v = enemy[v];

		if (enemy_u >= 0 || enemy_v >= 0)
		{
			int mergeEnemy = merge(enemy_u, enemy_v);
			enemy[mergeU] = mergeEnemy;
			enemy[mergeEnemy] = mergeU;
		}
		return true;
	}

	bool dis(int u, int v)
	{
		u = find(u);
		v = find(v);

		if (u == v)
			return false;

		int enemy_u = enemy[u];
		int enemy_v = enemy[v];

		int mergeA = merge(u, enemy_v);
		int mergeB = merge(v, enemy_u);

		enemy[mergeA] = mergeB;
		enemy[mergeB] = mergeA;
		return true;
	}
public:
	vector<int> parent, rank;
	vector<int> enemy, memberSize;
};
//---------------------------------------------------------------------------------------------

string maxPartyParticipants(int memberCnt, int commentCnt)
{
	char result_buf[100];

	bool contradiction = false;
	int contradicIndex = 0;
	DisjointSet disjointSet(memberCnt);
	for (int index = 0; index < commentCnt; index++)
	{
		char commentType[5];
		int u, v;
		fscanf(fpInput, "%s %d %d", commentType, &u, &v);

		if (contradiction)
			continue;

		if (strcmp(commentType, "ACK") == 0)
			contradiction = disjointSet.ack(u, v) ^ 1;
		else
			contradiction = disjointSet.dis(u, v) ^ 1;

		if (contradiction)
			contradicIndex = index + 1;
	}

	if (contradiction)
	{
		sprintf(result_buf, "CONTRADICTION AT %d\n", contradicIndex);
		return string(result_buf);
	}

	int ret = 0;
	vector<bool> checked(memberCnt, false);
	for (int index = 0; index < memberCnt; index++)
	{
		if (checked[index])
			continue;

		if (disjointSet.find(index) == index)
		{
			int mySize = disjointSet.getSize(index);

			int enemy = disjointSet.getEnemy(index);
			int enemySize = 0;
			if (enemy >= 0)
			{
				enemy = disjointSet.find(enemy);
				enemySize = disjointSet.getSize(enemy);
				checked[enemy] = true;
			}
			ret += max(enemySize, mySize);
		}
	}

	sprintf(result_buf, "MAX PARTY SIZE IS %d\n", ret);
	return string(result_buf);
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
		// 전역변수 초기화
		int memberCnt, commentCnt;
		fscanf(fpInput, "%d %d\n", &memberCnt, &commentCnt);
		string result = maxPartyParticipants(memberCnt, commentCnt);
		fprintf(fpOutput, "%s", result.c_str());
		if (isFile)
			printf("%s", result.c_str());
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