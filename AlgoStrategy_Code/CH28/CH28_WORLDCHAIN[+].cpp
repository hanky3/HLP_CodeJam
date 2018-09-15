#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;
#define MAX_SIZE			11
typedef long long int64;

int N;
vector<string> words;
vector<vector<int>> adjWords;
vector<int> inDegree, outDegree;

void readInputData()
{
	char buf[MAX_SIZE];
	fscanf(fpInput, "%d", &N);
	words = vector<string>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%s", buf);
		words[i] = buf;
	}
}

void makeGraph()
{
	adjWords = vector<vector<int>>(26, vector<int>(26, 0));
	inDegree = vector<int>(26, 0);
	outDegree = vector<int>(26, 0);

	for (auto w : words) {
		int start = *w.begin() - 'a';
		int end = w.back() - 'a';
	
		adjWords[start][end]++;
		outDegree[start]++;
		inDegree[end]++;
	}
}

void makeEulerCircuit(int here, vector<int> &circuit)
{
	for (int there = 0; there < adjWords.size(); there++) {
		while (adjWords[here][there] > 0) {
			adjWords[here][there]--;
			makeEulerCircuit(there, circuit);
		}
	}
	circuit.push_back(here);
}

bool checkEuler(bool &circuit, int &u)
{
	int plus1 = 0, minus1 = 0;
	for (int here = 0; here < 26; here++) {
		int delta = inDegree[here] - outDegree[here];
		if (delta == 1) {
			plus1++;
		}
		else if (delta == -1) {
			minus1++;
			u = here;
		}
		else if (delta != 0) {
			return false;
		}
		if (minus1 == 0 && outDegree[here])
			u = here;
	}

	if (plus1 == 0 && minus1 == 0) {
		circuit = true;
		return true;
	}
	if (plus1 == 1 && minus1 == 1) {
		circuit = false;
		return true;
	}
	return false;
}

string findWordChain()
{
	int u;
	bool isCircuit;
	vector<int> circuits;
	string ret;

	if (!checkEuler(isCircuit, u)) {
		return "IMPOSSIBLE";
	}

	makeEulerCircuit(u, circuits);
	reverse(circuits.begin(), circuits.end());

	char start = '\0';
	for (auto v : circuits) {
		char end = v + 'a';
		if (start != '\0') {
			auto itr = words.begin();
			while(itr != words.end()) {
				auto w = *itr;
				if (w[0] == start && w.back() == end) {
					ret.append(w + " ");
					words.erase(itr);
					break;
				}
				itr++;
			}
		}
		start = end;
	}
	ret.pop_back();
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
	fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		makeGraph();
		auto ret = findWordChain();
#ifdef _FILE_
		printf("%s\n", ret.c_str());
#endif
		fprintf(fpOutput, "%s\n", ret.c_str());
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
