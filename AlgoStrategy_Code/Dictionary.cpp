#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#if 0
using namespace std;

#define MAX_WIDTH 10
int cache[MAX_WIDTH];

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

vector<vector<int>> adjList;
vector<int> visited;
vector<int> ordered;
void makeGraph(vector<string> inputWords)
{
	int alphaLength = (int)'z' - (int)'a' + 1;
	adjList.clear();
	adjList.resize(alphaLength, vector<int>(alphaLength, 0));

	string prev = "";
	for (auto word : inputWords)
	{
		if (prev != "")
		{
			int minLength = min(prev.length(), word.length());
			for (int index = 0; index < minLength; index++)
			{
				if (prev[index] != word[index])
				{
					adjList[prev[index] - 'a'][word[index] - 'a'] = 1;
					break;
				}
			}
		}
		prev = word;
	}
}

void dfs(int u)
{
	vector<int> adj = adjList[u];

	visited[u] = true;
	for (int i = 0; i < adj.size(); i++)
	{
		if (adj[i] && !visited[i])
			dfs(i);
	}
	ordered.push_back(u);
}

string dfsAll()
{
	int alphaLength = 'z' - 'a' + 1;

	string path;
	ordered.clear();
	visited.clear();
	visited.resize(alphaLength, 0);
	for (int index = 0; index < alphaLength; index++)
	{
		if (!visited[index])
			dfs(index);
	}
	reverse(ordered.begin(), ordered.end());

	for (int i = 0; i < alphaLength; ++i){
		for (int j = i + 1; j < alphaLength; ++j){
			if (adjList[ordered[j]][ordered[i]])
				return "INVALID HYPOTHESIS";
		}
	}

	for (auto ch : ordered)
		path.push_back((char)(ch + 'a'));

	return path;
}

string remainAlphabet(string omitAlphabet)
{
	string alphabet;

	for (char ch = 'a'; ch <= 'z'; ch++)
	{
		if (omitAlphabet.find(ch) == string::npos)
		{
			alphabet.push_back(ch);
		}
	}
	return alphabet;
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

	char buf[20];
	for (int i = 0; i < testcase; i++)
	{
		int wordCount;
		fscanf(fpInput, "%d", &wordCount);

		vector<string> inputWords;
		for (int i = 0; i < wordCount; i++)
		{
			fscanf(fpInput, "%s", buf);
			inputWords.push_back(buf);
		}

		makeGraph(inputWords);
		string result = dfsAll();
		fprintf(fpOutput, "%s\n", result.c_str());
		if (isFile)
			printf("%s\n", result.c_str());
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
#endif