#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <set>


using namespace std;
typedef long long int64;
FILE *fpInput;
FILE *fpOutput;

#define MAX_BUF_SIZE		1000001

string sentence;


void readInputData()
{
	while (!feof(fpInput)) {
		char ch;
		fscanf(fpInput, "%c", &ch);
		if (ch == '\n')
			break;

		sentence.push_back(ch);
	}
}


int calcWordCount()
{
	int ret = 0;

	set<string> wordList;
	auto itr = sentence.begin();
	string currWord;
	while (itr != sentence.end()) {
		char ch = *itr;

		if (ch == ' ') {
			if (currWord.length() > 0)
				ret++;
			currWord.clear();
		}
		else {
			currWord.push_back(ch);
		}
		itr++;
	}
	if (currWord.length() > 0) {
		ret++;
	}

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
		auto ret = calcWordCount();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
