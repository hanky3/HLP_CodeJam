#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
#define MAX_TEXT_SIZE	1000000

FILE *fpInput;
FILE *fpOutput;

string T, P;
void readStringVal(string &val)
{
	char ch;
	val.clear();
	while (!feof(fpInput)) {
		fscanf(fpInput, "%c", &ch);
		if (ch == '\n')
			break;
		val.push_back(ch);
	}
}

void readInputData()
{
	readStringVal(T);
	readStringVal(P);
}

vector<int> getPartialMatch(const string &N)
{
	int n = N.size();
	vector<int> pi(N.size(), 0);

	int begin = 1, matched = 0;
	while (begin + matched < n) {
		if (N[begin + matched] == N[matched]) {
			matched++;
			pi[begin + matched - 1] = matched;
		}
		else {
			if (matched == 0)
				begin++;
			else {
				begin += matched - pi[matched - 1];
				matched = pi[matched - 1];
			}
		}
	}

	return pi;
}

vector<int> kmpSearch(const string& H, const string &N)
{
	int n = H.size();
	int m = N.size();
	vector<int> ret;

	vector<int> pi = getPartialMatch(N);
	int begin = 0, matched = 0;
	while (begin <= n - m) {
		if (matched < m && H[begin + matched] == N[matched]) {
			matched++;
			if (matched == m)
				ret.push_back(begin);
		}
		else {
			if (matched == 0)
				begin++;
			else {
				begin += matched - pi[matched - 1];
				matched = pi[matched - 1];
			}
		}
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
		auto ret = kmpSearch(T, P);
#ifdef _FILE_
		printf("%d\n", ret.size());
#endif
		fprintf(fpOutput, "%d\n", ret.size());
		for (auto s : ret) {
#ifdef _FILE_
			printf("%d ", (s+1));
#endif
			fprintf(fpOutput, "%d ", (s + 1));
		}
#ifdef _FILE_
		printf("\n");
#endif
		fprintf(fpOutput, "\n");

	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
