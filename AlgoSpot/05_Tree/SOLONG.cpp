#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <time.h>
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

//typedef struct KeywordTree KeywordTree;

#define NULL_IDX 26
class KeywordTree
{
public :
	KeywordTree(char c, string word, int freq) : ch(c), autoKeyword(word), frequency(freq), childTrees(NULL_IDX+1)
	{
		for (int index = 0; index < childTrees.size(); index++)
			childTrees[index] = NULL;
	}

	void updateAutoKeyword(pair<string, int> keywordInfo)
	{
		bool updated = false;
		if (keywordInfo.second > frequency)
			updated = true;
		else if (keywordInfo.second == frequency)
		{
			if (strcmp(keywordInfo.first.c_str(), autoKeyword.c_str()) < 0)
				updated = true; 
		}

		if (updated)
		{
			autoKeyword = keywordInfo.first;
			frequency = keywordInfo.second;
		}
	}

public :
	char ch;
	string autoKeyword;
	int frequency;

	vector<KeywordTree*> childTrees;
};
KeywordTree *rootKeywordTree  = NULL;
int dictionaryCnt, keywordCnt;

void releaseKeywordTree(KeywordTree *root)
{
	if (root == NULL) return;

	for (int index = 0; index < root->childTrees.size(); index++)
		releaseKeywordTree(root->childTrees[index]);

	free(root);
}

void addKeywordTree(KeywordTree *root, string keyword, int frequency)
{
	if (keyword.empty())
	{
		if (root->childTrees[NULL_IDX] == NULL)
			root->childTrees[NULL_IDX] = new KeywordTree('\0', "", frequency);
		return;
	}

	char currCh = keyword.front();
	int nextIndex = currCh - 'A';
	keyword.erase(keyword.begin());

	KeywordTree *childTree = root->childTrees[nextIndex];
	if (childTree != NULL)
		root->childTrees[nextIndex]->updateAutoKeyword(make_pair(keyword, frequency));
	else
		root->childTrees[nextIndex] = new KeywordTree(currCh, keyword, frequency);

	addKeywordTree(root->childTrees[nextIndex], keyword, frequency);
}

void makeKeywordTrees(KeywordTree *root, int dictionaryCnt)
{
	for (int index = 0; index < dictionaryCnt; index++)
	{
		char keyword[11];
		int frequency;

		fscanf(fpInput, "%s %d\n", keyword, &frequency);
		addKeywordTree(root, string(keyword), frequency);
	}
}

int minInputCount(KeywordTree *root, string keyword)
{
	if (keyword.empty())
		return 0;
	if (root->autoKeyword == keyword)
		return 1;

	int ret = 0;
	int nextIndex = keyword.front() - 'A';

	KeywordTree *nextChild = root->childTrees[nextIndex];
	if (nextChild == NULL)
		return keyword.size();

	keyword.erase(keyword.begin());
	ret = minInputCount(nextChild, keyword) + 1;
	return ret;
}

int sumOfMinimumInputs(KeywordTree *root, int keywordCount)
{
	int ret = 0;
	for (int index = 0; index < keywordCount; index++)
	{
		char keyword[11];
		fscanf(fpInput, "%s", keyword);
		ret += minInputCount(root, string(keyword));
		if (index < keywordCount - 1)
			ret++;
	}
	return ret;
}

void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");

	if (!isFile)
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else
	{
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++)
	{
		// 전역변수 초기화
		rootKeywordTree = new KeywordTree('\0', "", 0);

		fscanf(fpInput, "%d %d\n", &dictionaryCnt, &keywordCnt);
		makeKeywordTrees(rootKeywordTree, dictionaryCnt);
		int ret = sumOfMinimumInputs(rootKeywordTree, keywordCnt);
		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);

		releaseKeywordTree(rootKeywordTree);
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}