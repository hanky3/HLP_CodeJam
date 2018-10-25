#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>

using namespace std;

#define MAX_TEXT_SIZE	400001

FILE *fpInput;
FILE *fpOutput;

string fatherStr;
string motherStr;

void readInputData()
{
	char buf1[MAX_TEXT_SIZE];

	fscanf(fpInput, "%s\n", buf1);
	fatherStr = buf1;
	fscanf(fpInput, "%s\n", buf1);
	motherStr = buf1;
}

vector<int> getPartialMatch(string s)
{
	int n = s.size();
	int begin = 0, matched = 0;
	vector<int> partialMatch(n, 0);

	begin = 1;
	while (begin + matched < n) {
		if (s[begin + matched] == s[matched]) {
			matched++;
			partialMatch[begin + matched - 1] = matched;
		}
		else {
			if (matched == 0)
				begin++;
			else {
				begin += matched - partialMatch[matched - 1];
				matched = partialMatch[matched - 1];
			}
		}
	}

	return partialMatch;
}

vector<int> findPrePostMatch(string &a, string &b)
{
	string added = a + b;
	vector<int> p = getPartialMatch(added);
	vector<int> ret;

	int matched = added.length();
	while (matched > 0) {
		ret.push_back(matched);
		matched = p[matched-1];
	}
	reverse(ret.begin(), ret.end());
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
	//fscanf(fpInput, "%d", &testCase);
	while (testCase > 0) {
		readInputData();
		vector<int> ret = findPrePostMatch(fatherStr, motherStr);
		if (isFile) {
			for (auto n : ret) printf("%d ", n);
			printf("\n");
		}
		for (auto n : ret) fprintf(fpOutput, "%d ", n);
		fprintf(fpOutput, "\n");
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