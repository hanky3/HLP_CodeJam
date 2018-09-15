#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

#define MAX_NUMBER_SIZE	1000001

FILE *fpInput;
FILE *fpOutput;

int X;
string number;

void readInputData()
{
	char buf[MAX_NUMBER_SIZE];
	fscanf(fpInput, "%s", buf);
	number = buf;
}

pair<int, string> is3TimesNumber(string &num)
{
	int transforms = 0;
	while (num.size() > 1) {
		int sum = 0;
		for (int pos = 0; pos < num.size(); pos++)
			sum += (num[pos] - '0');

		num.clear();
		while (sum > 0) {
			num.insert(num.begin(), (sum % 10) + '0');
			sum /= 10;
		}

		transforms++;
	}

	pair<int, string> ret(transforms, "NO");
	if ((num[0]-'0') % 3 == 0)
		ret.second = "YES";
	return ret;
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	string outputFileName = string(argv[1]);
	outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
	fpOutput = fopen(outputFileName.c_str(), "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		int count = 0;
		auto ret = is3TimesNumber(number);
#ifdef _FILE_
		printf("%d\n%s\n", ret.first, ret.second.c_str());
#endif
		fprintf(fpOutput, "%d\n%s\n", ret.first, ret.second.c_str());
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
