#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_WORD_COUNT		510
#define MAX_Q_WORD_COUNT	110
#define MAX_WORD_SIZE		11

int wordCnt, queryCnt;
vector<vector<double>> nextWordFactors;
vector<vector<double>> wordClassifyFactors;
map<string, int> wordIndexes;
vector<string> wordList;

vector<double> firstWordFactors;
vector<vector<string>> queries;

int wordSelected[MAX_Q_WORD_COUNT][MAX_WORD_COUNT];
double cache[MAX_Q_WORD_COUNT][MAX_WORD_COUNT];

void readInputData()
{
	fscanf(fpInput, "%d %d", &wordCnt, &queryCnt);

	char buf[MAX_WORD_SIZE];
	wordIndexes.clear();
	wordList.clear();
	for (int index = 0; index < wordCnt; index++) {
		fscanf(fpInput, "%s", buf);
		wordIndexes.insert(make_pair(buf, index));
		wordList.push_back(buf);
	}

	firstWordFactors = vector<double>(wordCnt, 0.0f);
	for (int index = 0; index < wordCnt; index++) {
		fscanf(fpInput, "%lf", &firstWordFactors[index]);
	}

	nextWordFactors = vector<vector<double>>(wordCnt, vector<double>(wordCnt, 0.0f));
	for (int y = 0; y < wordCnt; y++)
		for (int x = 0; x < wordCnt; x++)
			fscanf(fpInput, "%lf", &nextWordFactors[y][x]);

	wordClassifyFactors = vector<vector<double>>(wordCnt, vector<double>(wordCnt, 0.0f));
	for (int y = 0; y < wordCnt; y++)
		for (int x = 0; x < wordCnt; x++)
			fscanf(fpInput, "%lf", &wordClassifyFactors[y][x]);

	//wordSelected = vector<vector<int>>(wordCnt+1, vector<int>(wordCnt+1, 0));

	queries = vector<vector<string>>(queryCnt);
	for (int index = 0; index < queryCnt; index++) {
		int count;
		fscanf(fpInput, "%d", &count);

		queries[index].clear();
		for (int index2 = 0; index2 < count; index2++) {
			fscanf(fpInput, "%s", buf);
			queries[index].push_back(buf);
		}
	}

	memset(cache, -1, sizeof(cache));
}


double maxTextPriority(int pos, int selected, vector<string> &queries)
{
	if (pos == queries.size())
		return 1.0f;

	double &ret = cache[pos][selected];

	if (ret >= 0.0f)
		return ret;

	ret = 0.0f;

	int qWord = wordIndexes[queries[pos]];
	int realWord = 0;
	for (int next = 0; next < wordCnt; next++) {
		double classifyFactor, nextFactor;

		if (pos == 0)
			nextFactor = firstWordFactors[next];
		else
			nextFactor = nextWordFactors[selected - 1][next];
		classifyFactor = wordClassifyFactors[next][qWord];

		double probability = (classifyFactor * nextFactor) * maxTextPriority(pos+1, next+1, queries);
		//ret = max<double>(ret)
		if (probability > ret) {
			ret = probability;
			realWord = next;
		}
	}

	wordSelected[pos][selected] = realWord;

	return ret;
}

void reconstruct(int pos, int selected, int querySize, vector<string> &words)
{
	if (pos == querySize)
		return;

	int wordPos = wordSelected[pos][selected];
	words.push_back(wordList[wordPos]);
	reconstruct(pos+1, wordPos+1, querySize, words);
}

void generateOutput(bool isFile)
{
	for (auto queryWords : queries) {
		vector<string> words;
		words.clear();
		memset(cache, -1, sizeof(cache));
		memset(wordSelected, -1, sizeof(wordSelected));

		maxTextPriority(0, 0, queryWords);
		reconstruct(0, 0, queryWords.size(), words);

		string result;
		for (auto word : words) {
			result.append(word + " ");
		}
		result.pop_back();

		if (isFile) printf("%s\n", result.c_str());
		fprintf(fpOutput, "%s\n", result.c_str());
	}
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

	readInputData();
	generateOutput(isFile);

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}