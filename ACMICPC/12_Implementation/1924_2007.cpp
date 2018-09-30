#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

const string days[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
const int monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
int x, y;

void readInputData()
{
	fscanf(fpInput, "%d %d", &x, &y);
}

string toDayOfWeek(int month, int day)
{
	int dayPassed = 0;

	for (int m = 0; m < month-1; m++) {
		dayPassed += monthDays[m];
	}
	dayPassed += day;
	return days[(dayPassed) % 7];
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
		auto ret = toDayOfWeek(x, y);
#ifdef _FILE_
		printf("%s\n", ret.c_str());
#endif
		fprintf(fpOutput, "%s\n", ret.c_str());
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
