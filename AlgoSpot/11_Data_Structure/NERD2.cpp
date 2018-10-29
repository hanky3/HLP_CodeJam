#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <math.h>
#include <assert.h>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N;
//priority_queue<pair<int, int>, greater<pair<int, int>>> nerdInfo;
vector<pair<int, int>> nerdInfo;


void readInputData()
{
	fscanf(fpInput, "%d", &N);

	nerdInfo = vector<pair<int, int>>(N, make_pair(0, 0));
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d %d", &nerdInfo[index].first, &nerdInfo[index].second);
	}
}

bool isDominant(pair<int, int> &p, vector<pair<int, int>> &realNerds)
{
	auto itr = lower_bound(realNerds.begin(), realNerds.end(), p);
	if (itr == realNerds.end())
		return false;

	return (itr->second > p.second);
}

long long findNerdCount()
{
	long long ret = 0;

	vector<pair<int, int>> realNerds;
	for (auto nerd : nerdInfo) {
		//NerdInfo newNerd(nerd.first, nerd.second);
		
		if (realNerds.empty()) {
			realNerds.push_back(nerd);
			ret += realNerds.size();
			continue;
		}

		if (isDominant(nerd, realNerds)) {
			ret += realNerds.size();
			continue;
		}

		auto itr = lower_bound(realNerds.begin(), realNerds.end(), nerd);
		if (itr != realNerds.begin()) {
			--itr;
			while (true) {
				if (itr->second > nerd.second) {
					++itr;
					break;
				}

				if (itr == realNerds.begin()) {
					realNerds.erase(itr);
					break;
				}
				else {
					auto nextItr = itr;
					nextItr--;
					realNerds.erase(itr);
					itr = nextItr;
				}
			}
		}


		if (realNerds.empty())
			realNerds.push_back(nerd);
		else
			realNerds.insert(itr, nerd);

		ret += realNerds.size();
	}

	return ret;
}

void solveProblem(const char *fileName, bool isFile)
{
	fpInput = stdin;
	fpOutput = stdout;
	if (isFile) {
		fpInput = fopen(fileName, "r");
		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}

	int testCase = 0;

	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		long long ret = findNerdCount();
		if (isFile)
			printf("%lld\n", ret);
		fprintf(fpOutput, "%lld\n", ret);

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}