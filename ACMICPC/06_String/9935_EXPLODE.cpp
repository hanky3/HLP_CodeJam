#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
#define MAX_TEXT_SIZE				1000000
#define MAX_KEYWORD_SIZE	36

FILE *fpInput;
FILE *fpOutput;

string textStr;
string keywordStr;


void readInputData()
{
	char buf[MAX_TEXT_SIZE + 1];
	fscanf(fpInput, "%s", buf);
	textStr = buf;
	fscanf(fpInput, "%s", buf);
	keywordStr = buf;
}


string explodeText(string &src, string &keyword)
{
	int begin = 0, matched = 0;
	string ret;
	vector<string> pi;

	for (auto ch : src) {
		ret.push_back(ch);

		if (ch == keyword.back() && ret.size() >= keyword.size()) {
			bool matched = true;
			for (int pos = 0; pos < keyword.size(); pos++) {
				if (ret[ret.size() - 1 - pos] != keyword[keyword.size() - 1 - pos]) {
					matched = false;
					break;
				}
			}
			if (matched) {
				ret.erase(ret.begin() + (ret.size()-keyword.size()), ret.end());
			}
		}
	}

	if (ret.empty())
		ret = "FRULA";
	return ret;
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
		auto ret = explodeText(textStr, keywordStr);
#ifdef _FILE_
		printf("%s\n", ret.c_str());
#endif
		fprintf(fpOutput, "%s\n", ret.c_str());
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}