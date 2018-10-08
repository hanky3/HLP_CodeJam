#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>


using namespace std;
FILE *fpInput;
FILE *fpOutput;

int N;
vector<vector<int>> gameMap;
pair < int, int > directions[] = { { 1, 0 }, { 1, 1 }, {0, 0}, { 0, 1 } };

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	gameMap = vector<vector<int>>(N, vector<int>(N));
	for (int h = 0; h < N; h++) {
		for (int w = 0; w < N; w++) {
			fscanf(fpInput, "%d", &gameMap[h][w]);
		}
	}
}

vector<vector<int>> changeBlocks(int dir, vector<vector<int>> &blocks)
{
	vector<vector<int>>newBlockMap(N, vector<int>(N, 0));
	int up = directions[dir].first;
	int sgn = directions[dir].second;


	for (int h = 0; h < N; h++) {
		vector<pair<int, bool>> merged;
		for (int sub = 0; sub < N; sub++) {
			int subPos = sgn ? (N - sub - 1) : sub;
			int &curr = up ? blocks[subPos][h] : blocks[h][subPos];

			if (curr == 0) continue;
			if (!merged.empty() && !merged.back().second && (merged.back().first == curr)) {
				merged.back().second = true;
				merged.back().first += curr;
			}
			else {
				merged.push_back({ curr, false });
			}
		}

		auto itr = merged.begin();
		for (int sub = 0; sub < N; sub++) {
			int subPos = sgn ? (N - sub - 1) : sub;
			int &curr = up ? newBlockMap[subPos][h] : newBlockMap[h][subPos];
		
			curr = 0;
			if (itr != merged.end()) {
				curr = itr->first;
				itr++;
			}
		}
	}

	return newBlockMap;
}


int maxBlockNumber(int step, vector<vector<int>> &blockMap)
{
	int ret = 0;
	if (step == 5) {
		for (int h = 0; h < N; h++) {
			for (int w = 0; w < N; w++) {
				ret = max(ret, blockMap[h][w]);
			}
		}
		return ret;
	}

	for (int dir = 0; dir < 4; dir++) {
		auto nextBlockMap = changeBlocks(dir, blockMap);
		ret = max(ret, maxBlockNumber(step + 1, nextBlockMap));
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
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = maxBlockNumber(0, gameMap);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
