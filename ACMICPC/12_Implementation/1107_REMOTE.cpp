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

FILE *fpInput;
FILE *fpOutput;

enum E_NUM_TYPE { E_EVEN=0, E_UP, E_DOWN};

int N, M;
vector<bool> wrongButtons;
vector<int> buttons;
int cache[10][3];
int numSize;
string numStr;

string toNumString(int num, int &size)
{
	char buf[10];
	sprintf(buf, "%d", num);

	string ret = buf;
	size = ret.size();
	return ret;
}

void readInputData()
{
	memset(cache, -1, sizeof(cache));
	fscanf(fpInput, "%d %d", &N, &M);

	wrongButtons = vector<bool>(10, false);
	for (int index = 0; index < M; index++) {
		int num;
		fscanf(fpInput, "%d", &num);
		wrongButtons[num] = true;
	}

	buttons.clear();
	for (int n = 0; n <= 9; n++) {
		if (wrongButtons[n]) continue;
		buttons.push_back(n);
	}
	numStr = toNumString(N, numSize);
}

bool isAvailableNum(int num, int &size)
{
	string numStrVal = toNumString(num, size);
	for (auto digit : numStrVal) {
		if (wrongButtons[digit - '0'])
			return false;
	}
	return true;
}

int minButtonPress()
{
	int btnPressed = abs(N - 100);
	if (buttons.size() == 0 || btnPressed == 0)
		return btnPressed;

	for (int num = 0; num <= 1000000; num++) {
		int length;
		if (isAvailableNum(num, length)) {
			btnPressed = min(btnPressed, length + abs(N - num));
			if (num >= N)	
				break;
		}
	}
	return btnPressed;
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
		auto ret = minButtonPress();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}