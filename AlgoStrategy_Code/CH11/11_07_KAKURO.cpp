#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#if 0
using namespace std;

#define MAX_OBJECT 101
#define MAX_WEIGHT 1001
int cache[MAX_OBJECT][MAX_WEIGHT];
unsigned long long cacheObjs[MAX_OBJECT][MAX_WEIGHT][2];

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

vector<pair<int, int>> travelObjList;
vector<string> travelObjNames;

int getSize(int set)
{
	int size = 0;
	for (int val = 1; val <= 9; val++)
	{
		if (set & (1 << val))
			size++;
	}
	return size;
}

int getSum(int set)
{
	int sum = 0;
	for (int val = 1; val <= 9; val++)
	{
		if (set & (1 << val))
			sum += val;
	}
	return sum;
}

int candidates[10][46][1024];
void generateCandidates()
{
	memset(candidates, 0, sizeof(candidates));

	for (int set = 0; set < 1024; set += 2)
	{
		int l = getSize(set);
		int s = getSum(set);

		int subset = set;
		while (true)
		{
			candidates[l][s][subset] |= (set & ~subset);
			if (subset == 0)
				break;
			subset = (subset - 1) & set;
		}
	}
}

#define MAXN 20
int n, color[MAXN][MAXN], value[MAXN][MAXN], hint[MAXN][MAXN][2];
int q, sum[MAXN*MAXN*2], length[MAXN*MAXN*2], known[MAXN*MAXN*2];
void put(int y, int x, int val)
{
	for (int h = 0; h < 2; ++h)
		known[hint[y][x][h]] += (1 << val);
	value[y][x] = val;
}

void remove(int y, int x, int val)
{
	for (int h = 0; h < 2; ++h)
		known[hint[y][x][h]] -= (1 << val);
	value[y][x] = 0;
}

int getCandHint(int hint)
{
	return candidates[length[hint]][sum[hint]][known[hint]];
}

int getCandCoord(int y, int x)
{
	return getCandHint(hint[y][x][0]) & getCandHint(hint[y][x][1]);
}

bool search()
{
	int y = -1, x = -1, minCands = 1023;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (color[i][j] == 1 && value[i][j] == 0) {
				int cands = getCandCoord(i, j);
				if (getSize(minCands) > getSize(cands))
				{
					minCands = cands;
					y = i;
					x = j;
				}
			}
		}
	}

	if (minCands == 0)
		return false;

	if (y == -1)
		return true;

	for (int val = 1; val <= 9; val++)
	{
		if (minCands & (1 << val))
		{
			put(y, x, val);
			if (search())
				return true;
			remove(y, x, val);
		}
	}
	return false;
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
		fscanf(fpInput, "%d", &n);

		memset(color, 0, sizeof(color));
		memset(value, 0, sizeof(value));
		memset(hint, 0, sizeof(hint));
		memset(sum, 0, sizeof(sum));
		memset(length, 0, sizeof(length));
		memset(known, 0, sizeof(known));
	
		generateCandidates();
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				fscanf(fpInput, "%d", &color[i][j]);

		fscanf(fpInput, "%d", &q);

		for (int i = 0; i < q; i++)
		{
			int y, x, direction, total;
			fscanf(fpInput, "%d %d %d %d", &y, &x, &direction, &total);
			int hintVal = (y - 1) * 20 + (x - 1) + (400 * direction);
			length[hintVal] = 0;
			sum[hintVal] = total;
			int startY = y, startX = x;
			if (direction == 1)
				startX--;
			else
				startY--;

			while (color[startY][startX])
			{
				hint[startY][startX][direction] = hintVal;
				length[hintVal]++;

				if (direction == 1)
					startY++;
				else
					startX++;
			}
		}
		//reconstruct(maxWeight, 0, objNames);
		
		bool result = search();

		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				fprintf(fpOutput, "%d ", value[y][x]);
			}
			fprintf(fpOutput, "\n");
		}
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
#endif