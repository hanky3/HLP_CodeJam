#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>

using namespace std;

#define MAX_TEXT_SIZE	101

FILE *fpInput;
FILE *fpOutput;

string parenStr;
string parenOrder;
map<char, char> reverseParen;
map<char, int> leftParenOrder;

void readInputData()
{
	char buf1[MAX_TEXT_SIZE], buf2[MAX_TEXT_SIZE];

	reverseParen.clear();
	reverseParen.insert(make_pair(')', '('));
	reverseParen.insert(make_pair('}', '{'));
	reverseParen.insert(make_pair(']', '['));
	reverseParen.insert(make_pair('>', '<'));
	reverseParen.insert(make_pair('(', ')'));
	reverseParen.insert(make_pair('{', '}'));
	reverseParen.insert(make_pair('[', ']'));
	reverseParen.insert(make_pair('<', '>'));


	fscanf(fpInput, "%s %s\n", buf1, buf2);
	parenStr = buf1;

	string leftOrder = buf2;
	leftParenOrder.clear();
	for (int pos = 0; pos < leftOrder.length(); pos++) {
		leftParenOrder.insert(make_pair(leftOrder[pos], pos));
	}
}

bool isLeftParen(char paren)
{
	if (paren == '{' || paren == '(' || paren == '[' || paren == '<')
		return true;
	return false;
}

string fixParenthesis(string &before)
{
	string fixed = before;

	stack<pair<char, int>> st;
	
	for (int pos = 0; pos < before.length(); pos++) {
		if (isLeftParen(before[pos])) {
			st.push(make_pair(before[pos], pos));
			continue;
		}

		char left = st.top().first;
		char right = before[pos];
		int leftPos = st.top().second;
		st.pop();

		if (left == reverseParen[right])
			continue;
		
		if (leftParenOrder[left] < leftParenOrder[reverseParen[right]])
			fixed[pos] = reverseParen[left];
		else
			fixed[leftPos] = reverseParen[right];
	}

	return fixed;
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
	while (testCase > 0) {
		readInputData();
		string ret = fixParenthesis(parenStr);
		if (isFile)
			printf("%s\n", ret.c_str());
		fprintf(fpOutput, "%s\n", ret.c_str());
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