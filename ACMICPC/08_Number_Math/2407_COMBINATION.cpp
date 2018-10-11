#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>


using namespace std;
FILE *fpInput;
FILE *fpOutput;
#define MAX_NUM		(101)
typedef unsigned long long uint64;

int N, M;
vector<int> cache[MAX_NUM][MAX_NUM];

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);
}

vector<int> addNumValues(vector<int> &a, vector<int> &b)
{
	int size = max(a.size(), b.size());
	vector<int> added(size + 1, 0);
	for (int pos = 0; pos < size; pos++) {
		if (pos < a.size()) added[pos] += a[pos];
		if (pos < b.size()) added[pos] += b[pos];

		added[pos + 1] += added[pos] / 10;
		added[pos] = added[pos] % 10;
	}
	while (true) {
		if (added.size() == 1) break;
		if (added.back() != 0) break;
		added.pop_back();
	}
	return added;
}

void precalcBino(int k)
{
	for (int j = 0; j <= k; j++)
		cache[0][j] = vector<int>(1, 0);

	for (int i = 1; i <= k; i++) {
		for (int j = 0; j <= k; j++) {
			if (j == 0 || i == j) {
				cache[i][j] = vector<int>(1, 1);
				continue;
			}
			cache[i][j] = addNumValues(cache[i - 1][j - 1], cache[i - 1][j]);
		}
	}
}


vector<int> calcBino(int n, int m)
{
	return cache[n][m];
}

string toNumStr(vector<int> &num)
{
	string ret;
	for (auto n : num) {
		ret.insert(ret.begin(), (n + '0'));
	}
	return ret;
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
	precalcBino(100);
	while (testCase-- > 0) {
		readInputData();
		auto ret = calcBino(N, M);
		string strVal = toNumStr(ret);
#ifdef _FILE_
		printf("%s\n", strVal.c_str());
#endif
		fprintf(fpOutput, "%s\n", strVal.c_str());
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
