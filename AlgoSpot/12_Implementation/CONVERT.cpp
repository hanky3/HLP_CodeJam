#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

map<string, double> convertTo;
map<string, string> unitConvert;
double unitVal;
string unitStr;

void initConvertValue()
{
	convertTo.clear();
	convertTo.insert(make_pair("lb", 2.2046));
	convertTo.insert(make_pair("kg", 0.4536));
	convertTo.insert(make_pair("g", 0.2642));
	convertTo.insert(make_pair("l", 3.7854));

	unitConvert.clear();
	unitConvert.insert(make_pair("kg", "lb"));
	unitConvert.insert(make_pair("lb", "kg"));
	unitConvert.insert(make_pair("g", "l"));
	unitConvert.insert(make_pair("l", "g"));
}

void readInputData()
{
	char buf[10];
	fscanf(fpInput, "%lf %s", &unitVal, buf);
	unitStr = buf;
}

pair<string, double> convertUnitVal(string from, double val)
{
	string to = unitConvert[from];
	double factor = convertTo[to];

	double convertVal = val * factor;
	return make_pair(to, convertVal);
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

	initConvertValue();
	fscanf(fpInput, "%d", &testCase);
	int index = 1;
	while (testCase > 0)
	{
		readInputData();
		auto ret = convertUnitVal(unitStr, unitVal);
		if (isFile)
			printf("%d %.4f %s\n", index, ret.second, ret.first.c_str());
		fprintf(fpOutput, "%d %.4f %s\n", index, ret.second, ret.first.c_str());

		testCase--;
		index++;
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