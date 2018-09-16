#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N;
string numberStr;

void readInputData()
{
	char buf[100001];
	fscanf(fpInput, "%s", buf);
	numberStr = buf;
	sort(numberStr.begin(), numberStr.end(), [](char a, char b) {
		return (a > b);
	});
}

string generateMaxNum()
{
	auto itr = find(numberStr.begin(), numberStr.end(), '0');
	if (itr == numberStr.end())
		return "-1";

	int sum = 0;
	for (auto ch : numberStr) {
		int n = ch - '0';
		sum += n;
	}
	if (sum % 3 != 0)
		return "-1";

	return numberStr;
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
	while (testCase > 0)	{
		readInputData();
		auto ret = generateMaxNum();
#if _FILE_
		printf("%s\n", ret.c_str());
#endif
		fprintf(fpOutput, "%s\n", ret.c_str());
		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);

	return 0;
}
