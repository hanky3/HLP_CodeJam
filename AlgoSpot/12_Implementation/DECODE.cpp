#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_TEXT_SIZE		(401)
vector<char> TEXT_CHAR;
const pair<int, int> direction[4] = {
	make_pair(0, 1),
	make_pair(1, 0),
	make_pair(0, -1), 
	make_pair(-1, 0), 
};

int R, C;
vector<vector<char>> decodeMap;
vector<vector<bool>> visited;



void initConstValue()
{
	TEXT_CHAR = vector<char>(27);
	TEXT_CHAR[0] = ' ';

	for (int pos = 1; pos <= 26; pos++) {
		TEXT_CHAR[pos] = 'A' + pos - 1;
	}
}

void readInputData()
{
	fscanf(fpInput, "%d %d ", &R, &C);

	decodeMap = vector<vector<char>>(R, vector<char>(C));
	visited = vector<vector<bool>>(R, vector<bool>(C, false));

	for (int r = 0; r < R; r++) {
		for(int c = 0; c < C; c++) {
			fscanf(fpInput, "%c", &decodeMap[r][c]);
		}
	}
}

char converToChar(string decodeText)
{
	int index = 0;

	int pos = decodeText.size() - 1;
	for (auto ch : decodeText) {
		int val = ch - '0';
		index |= (val << pos);
		pos--;
	}
	return TEXT_CHAR[index];
}

string decodeData()
{
	string ret = "";
	string convertData;

	pair<int, int> currPos(0, 0);
	int currDir = 0;
	while (true) {
		if (currPos.first < 0 || currPos.first >= R || currPos.second < 0 || currPos.second >= C)
			break;

		if (visited[currPos.first][currPos.second])
			break;

		visited[currPos.first][currPos.second] = true;
		convertData.push_back(decodeMap[currPos.first][currPos.second]);

		pair<int, int> nextPos(currPos.first + direction[currDir].first, currPos.second + direction[currDir].second);

		bool dirChange = false;
		if (nextPos.first < 0 || nextPos.first >= R || nextPos.second < 0 || nextPos.second >= C)
			dirChange = true;
		else if (visited[nextPos.first][nextPos.second])
			dirChange = true;

		if (dirChange)
			currDir = (currDir + 1) % 4;

		currPos.first += direction[currDir].first;
		currPos.second += direction[currDir].second;
	}

	int index = 0;
	string buffer;
	for (auto ch : convertData) {
		buffer.push_back(ch);
		index++;
		if (index % 5 == 0) {
			char charData = converToChar(buffer);
			ret.push_back(charData);
			buffer.clear();
		}
	}

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

	int testCase = 0;

	initConstValue();
	fscanf(fpInput, "%d", &testCase);
	int index = 1;
	while (testCase > 0)
	{
		readInputData();
		auto ret = decodeData();
		if (isFile)
			printf("%d %s\n", index, ret.c_str());
		fprintf(fpOutput, "%d %s\n", index, ret.c_str());

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