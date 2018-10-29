#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_DIGIT	5

int N, M;
string numbers;

void readInputData()
{
	char buf[11];
	fscanf(fpInput, "%s %d %d", buf, &N, &M);

	numbers = buf;
	sort(numbers.begin(), numbers.end());
}

int calcModNumber(string &num, int divideNum)
{
	int mod = 0;

	for (auto n : num) {
		mod = (mod * 10) + (n-'0');
		mod %= divideNum;
	}

	return mod;
}

void eraseZeroPrefix(string &num)
{
	auto itr = num.begin();
	while (itr != num.end()) {
		if (*itr != '0')
			break;
		itr = num.erase(itr);
	}
}

bool isGreaterNum(string num1, string num2)
{
	eraseZeroPrefix(num1);
	if (num1.size() > num2.size())
		return true;
	else if (num2.size() > num1.size())
		return false;

	for (int pos = 0; pos < num1.size(); pos++) {
		if (num1[pos] > num2[pos])
			return true;
		else if (num1[pos] < num2[pos])
			return false;
	}
	return true;
}



string minNumber(int totalNum, int remains)
{
	string ret = "IMPOSSIBLE";
	vector<int> visited(totalNum, 0);

	queue<pair<int, string>> q;
	q.push(make_pair(0, ""));

	while (!q.empty()) {
		int r = q.front().first;
		string currNums = q.front().second;
		q.pop();

		if (visited[r] && r != remains)
			continue;
		visited[r] = true;

		if (r == remains) {
			char _total[MAX_DIGIT+1];
			sprintf(_total, "%d", (totalNum + remains));
			if (isGreaterNum(currNums, string(_total))) {
				eraseZeroPrefix(currNums);
				return currNums;
			}
		}

		for (auto n : numbers) {
			string nextNums = currNums + n;

			char _buf[11];
			memset(_buf, 0, sizeof(_buf));
			sprintf(_buf, "%d", (r * 10 + (n - '0')));
			string s = string(_buf);
			int mod = calcModNumber(s, totalNum);
			q.push(make_pair(mod, nextNums));
		}

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

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		string ret = minNumber(N, M);
		fprintf(fpOutput, "%s\n", ret.c_str());
		if (isFile)
			printf("%s\n", ret.c_str());

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