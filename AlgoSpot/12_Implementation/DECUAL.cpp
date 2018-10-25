#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_BUF_SIZE	1001

FILE *fpInput;
FILE *fpOutput;

int N, K;

string encStr1, encStr2;
vector<pair<string, int>> playerInfo1;
vector<pair<string, int>> playerInfo2;

vector<pair<string, int>> decodeString(string &str);
void readInputData()
{
	char buf[MAX_BUF_SIZE];
	fscanf(fpInput, "%s\n", buf);
	encStr1 = string(buf);

	fscanf(fpInput, "%s\n", buf);
	encStr2 = string(buf);

	playerInfo1 = decodeString(encStr1);
	playerInfo2 = decodeString(encStr2);
}

vector<pair<string, int>> decodeString(string &str)
{
	vector<pair<string, int>> ret;
	auto itr = str.begin();
	while (itr != str.end()) {
		char ch = *itr;
		pair<string, int> strInfo;

		if (ch == '(') {
			ch = *(++itr);
			while (ch != ')') {
				strInfo.first.push_back(ch);
				ch = *(++itr);
			}
			itr++; itr++;

			string num;
			while (itr != str.end()) {
				ch = *itr;
				if (ch < '0' || ch > '9')
					break;

				num.push_back(ch);
				itr++;
			}
			sscanf(num.c_str(), "%d", &strInfo.second);
			itr--;
		}
		else {
			strInfo.first.push_back(ch);
			strInfo.second = 1;
		}

		ret.push_back(strInfo);
		itr++;
	}
	return ret;
}

int gcd(int a, int b)
{
	if (b == 0)
		return a;
	return gcd(b, a%b);
}

bool reduceDuplicates(string &r1, string &r2, pair<string, int> &a, pair<string, int> &b)
{
	if (r1.size() >= a.first.size() || r2.size() >= b.first.size())
		return true;

	int gcdVal = gcd(a.first.size(), b.first.size());
	int aSize = b.first.size() / gcdVal;
	int bSize = a.first.size() / gcdVal;

	if (a.second < aSize || b.second < bSize)
		return true;

	string compare1, compare2;
	for (int pos = 0; pos < aSize; pos++) compare1.append(a.first);
	for (int pos = 0; pos < bSize; pos++) compare2.append(b.first);

	compare1 = r1 + compare1.substr(0, compare1.length() - r1.length());
	compare2 = r2 + compare2.substr(0, compare2.length() - r2.length());
	if (compare1 != compare2)
		return false;

	int multiply = min(a.second/aSize, b.second/bSize);
	a.second -= (aSize*multiply);
	b.second -= (bSize*multiply);
	return true;
}

bool stringMatching()
{
	bool ret = false;

	auto itrPlayer1 = playerInfo1.begin();
	auto itrPlayer2 = playerInfo2.begin();

	string remainStr1, remainStr2;
	while (itrPlayer1 != playerInfo1.end() || itrPlayer2 != playerInfo2.end()) {
		if (itrPlayer1 != playerInfo1.end() && remainStr1.size() == 0) {
			remainStr1.append(itrPlayer1->first);
			itrPlayer1->second--;
		}
		if (itrPlayer2 != playerInfo2.end() && remainStr2.size() == 0) {
			remainStr2.append(itrPlayer2->first);
			itrPlayer2->second--;
		}

		if (itrPlayer1 == playerInfo1.end() && remainStr1.empty())
			return false;
		if (itrPlayer2 == playerInfo2.end() && remainStr2.empty())
			return false;

		while (!remainStr1.empty() && !remainStr2.empty()) {
			if (remainStr1[0] != remainStr2[0])
				return false;
			remainStr1.erase(remainStr1.begin());
			remainStr2.erase(remainStr2.begin());
		}

		if (itrPlayer1 != playerInfo1.end() && itrPlayer2 != playerInfo2.end()) {
			if (!reduceDuplicates(remainStr1, remainStr2, *itrPlayer1, *itrPlayer2))
				return false;
		}

		if (itrPlayer1 != playerInfo1.end() && itrPlayer1->second == 0)
			itrPlayer1++;
		if (itrPlayer2 != playerInfo2.end() && itrPlayer2->second == 0)
			itrPlayer2++;
	}


	if (remainStr1.empty() && remainStr2.empty())
		return true;

	return false;
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
		bool ret = stringMatching();
		if (isFile)
			printf("%s\n", ret?"YES":"NO");
		fprintf(fpOutput, "%s\n", ret ? "YES" : "NO");
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