#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#if 0

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
#endif

#define MAX_NUM 200000

void normalize(vector<int>& num)
{
	num.push_back(0);
	for (size_t i = 0; i < num.size(); i++)
	{
		if (num[i] < 0)
		{
			int borrow = (abs(num[i]) + 9) / 10;
			num[i + 1] = borrow;
			num[i] += borrow * 10;
		}
		else if (num[i] > 0)
		{
			num[i + 1] += num[i] / 10;
			num[i] %= 10;
		}
	}
	if (num.back() == 0)
		num.pop_back();
}

vector<int> multiply(const vector<int>& a, const vector<int>& b)
{
	vector<int> c(a.size() + b.size() + 1, 0);
	for (size_t i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < b.size(); j++)
		{
			c[i + j] += a[i] * b[j];
		}
	}
	//normalize(c);

	return c;
} 

void addTo(vector<int> &a, const vector<int> &b, int k)
{
	a.resize(max(a.size(), b.size()) + k);
	for (size_t i = 0; i < b.size(); i++)
	{
		a[i+k] += b[i];
	}

	//normalize(a);
}

void subFrom(vector<int> &a, const vector<int> &b)
{
	a.resize(max(a.size(), b.size()) + 1);
	for (int i = 0; i < b.size(); i++)
	{
		a[i] -= b[i];
	}

	//normalize(a);
}

vector<int> karatsuba(const vector<int> &a, const vector<int> &b)
{
	int an = a.size(), bn = b.size();
	if (an < bn) 
		return karatsuba(b, a);
	if (an == 0 || bn == 0)
		return vector<int>();

	if (an <= 50)
		return multiply(a, b);

	int half = an / 2;

	vector<int> a0(a.begin(), a.begin() + half);
	vector<int> a1(a.begin() + half, a.end());
	vector<int> b0(b.begin(), b.begin() + half);
	vector<int> b1(b.begin() + half, a.end());

	vector<int> z2 = karatsuba(a1, b1);
	vector<int> z0 = karatsuba(a0, b0);

	addTo(a0, a1, 0);
	addTo(b0, b1, 0);

	vector<int> z1 = karatsuba(a0, b0);
	subFrom(z1, z2);
	subFrom(z1, z0);

	vector<int> ret;
	addTo(ret, z0, 0);
	addTo(ret, z1, half);
	addTo(ret, z2, half*2);

	return ret;
}
#if 0
int hugs(const string& members, const string& fans)
{
	int numOfHugs = 0;
	int memberSize = members.size();
	int fanSize = fans.size();

	vector<int> memberVec(memberSize, 0);
	vector<int> fanVec(fanSize, 0);

	for (int i = 0; i < memberSize; i++)
	{
		if (members[i] == 'M')
			memberVec[i] = 1;
	}

	for (int i = 0; i < fanSize; i++)
	{
		if (fans[i] == 'M')
			fanVec[fanSize - i - 1] = 1;
	}

	vector<int> result = karatsuba(memberVec, fanVec);

	for (int i = memberSize - 1; i < fanSize; i++)
	{
		if (!result[i])
			numOfHugs++;
	}
	return numOfHugs;
}

char buf[MAX_NUM];
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
		string member, fan;
		fscanf(fpInput, "%s", buf);
		member = string(buf);

		fscanf(fpInput, "%s", buf);
		fan = string(buf);

		int result = hugs(member, fan);
		fprintf(fpOutput, "%d\n", result);
		if (isFile)
			printf("%d\n", result);
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