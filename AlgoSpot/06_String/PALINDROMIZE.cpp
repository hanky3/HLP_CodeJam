#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_WORD_COUNT	100001
string inputWord;
string reverseWord;


vector<int> getPartialMatch(string &w)
{
	int m = w.size();
	vector<int> pi(m, 0);

	int begin = 1, matched = 0;
	while (begin + matched < m) {
		if (w[begin + matched] == w[matched]) {
			++matched;
			pi[begin + matched - 1] = matched;
		}
		else {
			if (matched == 0)
				begin++;
			else {
				begin += matched - pi[matched-1];
				matched = pi[matched - 1];
			}
		}
	}

	return pi;
}

void readInputData()
{
	char buf[MAX_WORD_COUNT];
	fscanf(fpInput, "%s", buf);

	inputWord = buf;

	reverseWord.clear();
	for (auto itr = inputWord.rbegin(); itr != inputWord.rend(); itr++)
		reverseWord.push_back(*itr);

	//cout << inputWord.c_str() << " " << reverseWord.c_str() << endl;
}

int shortestPalindrom()
{
	vector<int> pi = getPartialMatch(reverseWord);
	int m = inputWord.size();

	int begin = 0, matched = 0;
	while (begin < m) {
		if (inputWord[begin + matched] == reverseWord[matched]) {
			++matched;
			if (begin + matched == m)
				break;
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

	int palindromLength = (m * 2) - matched;
	return palindromLength;
}


void solveProblem(const char *fileName, bool isFile)
{
	if (isFile)
	{
		fpInput = fopen(fileName, "r");

		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}
	else
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	int testCase = 0;

	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();

		int ret = shortestPalindrom();
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}