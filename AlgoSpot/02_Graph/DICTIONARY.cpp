#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define ALPHABET_SIZE		26
#define MAX_WORD_SIZE		21

vector<string> words;
vector<vector<int>> alphaAdjs;
int firstAlphabet;
vector<int> alphabetOrder;
vector<bool> visited;


void readInputData()
{

	int wordCount;
	fscanf(fpInput, "%d", &wordCount);
	words = vector<string>(wordCount);
	alphaAdjs = vector<vector<int>>(ALPHABET_SIZE, vector<int>(ALPHABET_SIZE, 0));
	visited = vector<bool>(ALPHABET_SIZE, false);
	alphabetOrder.clear();

	for (int index = 0; index < wordCount; index++) {
		char buf[MAX_WORD_SIZE];
		fscanf(fpInput, "%s", buf);
		words[index] = buf;
	}
}

void makeAlphabetGraph()
{
	int prev = -1;

	for (int curr = 0; curr < words.size(); curr++) {
		string currWord = words[curr];

		if (prev >= 0) {
			string prevWord = words[prev];
			int lessWordSize = min(currWord.size(), prevWord.size());

			for (int pos = 0; pos < lessWordSize; pos++) {
				if (prevWord[pos] == currWord[pos])
					continue;

				alphaAdjs[prevWord[pos] - 'a'][currWord[pos] - 'a'] = 1;
				/*
				auto adjs = alphaAdjs[prevWord[pos] - 'a'];
				//if (find(adjs.begin(), adjs.end(), currWord[pos] - 'a') != adjs.end())
				//	continue;

				alphaAdjs[prevWord[pos] - 'a'].push_back(currWord[pos] - 'a');
				*/
				break;
			}
		}

		prev = curr;
	}

	return;
}

void dfs(int here)
{
	visited[here] = true;

	for (int there = 0; there < alphaAdjs[here].size(); there++) {
		if (alphaAdjs[here][there] && !visited[there])
			dfs(there);
	}

//	for (auto next : alphaAdjs[here]) {
//		if (!visited[next])
//			dfs(next);
//	}
	alphabetOrder.push_back(here);
}

bool topologicalSort(int v)
{
	bool ret = false;

	for (int alphabet = 0; alphabet < ALPHABET_SIZE; alphabet++) {
		if (!visited[alphabet])
			dfs(alphabet);
	}

	// 역방향 간선여부 체크
	reverse(alphabetOrder.begin(), alphabetOrder.end());
	for (int i = 0; i < alphabetOrder.size(); i++) {
		for (int j = i + 1; j < alphabetOrder.size(); j++) {
			if (alphaAdjs[alphabetOrder[j]][alphabetOrder[i]])
				return false;
			/*
			auto adjs = alphaAdjs[alphabetOrder[j]];
			if (find(adjs.begin(), adjs.end(), alphabetOrder[i]) != adjs.end())
				return false;
				*/
		}
	}

	return true;
}

string makeAlphabetOrder()
{
	string orderStr;
	for (auto alpha : alphabetOrder)
		orderStr.push_back(alpha + 'a');

	return orderStr;
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
	while (testCase > 0)
	{
		readInputData(); 
		makeAlphabetGraph();
		bool validOrder = topologicalSort(firstAlphabet);

		string ret = "INVALID HYPOTHESIS";
		if (validOrder) {
			ret = makeAlphabetOrder();
		}
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
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}