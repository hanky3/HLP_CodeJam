#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
using namespace std;

const double PI = 2.0 * acos(0.0);
FILE *fpInput = NULL;
FILE *fpOutput = NULL;

vector<int> getPartialMatch(const string &N)
{
	int n = N.size();
	vector<int> pi(N.size(), 0);

	int begin = 1, matched = 0;
	while (begin + matched < n)
	{
		if (N[begin + matched] == N[matched])
		{
			matched++;
			pi[begin + matched - 1] = matched;
		}
		else
		{
			if (matched == 0)
				begin++;
			else
			{
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
	while (begin <= n - m)
	{
		if (matched < m && H[begin + matched] == N[matched])
		{
			matched++;
			if (matched == m)
				ret.push_back(begin);
		}
		else
		{
			if (matched == 0)
				begin++;
			else
			{
				begin += matched - pi[matched - 1];
				matched = pi[matched - 1];
			}
		}
	}

	return ret;
}

int clockOverlap(const string &a, const string &b, bool clockWise)
{
	int n = a.size();
	int m = b.size();

	vector<int> pi = getPartialMatch(b);

	int begin = 0, matched = 0;
	while (begin < n)
	{
		if (matched < n && a[begin + matched] == b[matched])
		{
			matched++;
			if ((begin + matched) == m)
			{
				string prefixA = string(a.begin(), a.begin() + begin);
				string postfixB = string(b.begin() + matched, b.end());
				if (prefixA == postfixB)
					return (clockWise == true ? (matched%m) : begin);
				else
				{
					begin += matched - pi[matched - 1];
					matched = 0;
				}
			}
		}
		else
		{
			if (matched == 0)
				begin++;
			else
			{
				begin += matched - pi[matched - 1];
				matched = pi[matched - 1];
			}
		}
	}
	return 0;
}

int shifts(const string &original, const string &target)
{
	return kmpSearch(original + original, target)[0];
}

int findMinimumDialing(vector<string> dials)
{
	int ret = 0;
	bool clockWise = true;
	for (int i = 0; i < dials.size() - 1; i++)
	{
		if (!clockWise)
			ret += shifts(dials[i], dials[i + 1]);
		else
			ret += shifts(dials[i+1], dials[i]);

		clockWise = (1 ^ clockWise);
	}

	return ret;
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

	char buf[10001];
	for (int i = 0; i < testcase; i++)
	{
		int N = 0;
		fscanf(fpInput, "%d\n", &N);

		memset(buf, 0, 10001);
		vector<string> dials(N+1);
		for (int j = 0; j <= N; j++)
		{
			fscanf(fpInput, "%s", buf);
			dials[j] = buf;
		}

		int ret = findMinimumDialing(dials);

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