#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <set>

using namespace std;
FILE *fpInput, *fpOutput;
#define MAX_WORD_SIZE			1000001
#define ALPHABET_COUNT		26

string word;
int wordCount[ALPHABET_COUNT];

void readInputData()
{
	char buf[MAX_WORD_SIZE];
	memset(wordCount, 0, sizeof(wordCount));
	fscanf(fpInput, "%s\n", buf);
	word = buf;
}

char maxFreqAlphabet()
{
	for (auto w : word) {
		int pos = w - 'A';
		if (w >= 'a') 
			pos = w - 'a';
		wordCount[pos]++;
	}

	int cnt = 0;
	vector<char> maxFreqWords;
	for (int pos = 0; pos < ALPHABET_COUNT; pos++) {
		if (wordCount[pos] > cnt) {
			cnt = wordCount[pos];
			maxFreqWords.clear();
			maxFreqWords.push_back(pos + 'A');
		}
		else if (wordCount[pos] == cnt) {
			maxFreqWords.push_back(pos + 'A');
		}
	}

	if (maxFreqWords.size() > 1)
		return '?';

	return maxFreqWords[0];
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
	while (testCase > 0) {
		readInputData();
		auto ret = maxFreqAlphabet();
#ifdef _FILE_
		printf("%c\n", ret);
#endif
		fprintf(fpOutput, "%c\n", ret);
		testCase--;
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}
