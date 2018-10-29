#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

using namespace std;
#define INF 0x0FFFFFFF
#define MAX_V 1001

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

//class Compare2T 
//{
//public :
//	Compare2T(int _t, vector<int> _group) : t(_t), group(_group)
//	{}
//
//	bool operator() (int a, int b)
//	{
//		if (group[a] != group[b]) 
//			return (group[a] < group[b]);
//
//		return group[a + t] < group[b + t];
//	}
//	int t;
//	vector<int> group;
//};

int commonPrefix(string s, int i, int j)
{
	int k = 0;
	while (i < s.size() && j < s.size() && s[i] == s[j])
	{
		i++; j++;
		k++;
	}

	return k;
}

class Comparator {
public :
	vector<int> &group;
	int t;

	Comparator(vector<int> &_group, int _h) : group(_group), t(_h) { }

	bool operator () (int a, int b) {
		// 첫 t 글자가 다르면 이들을 이용해 비교한다
		if (group[a] != group[b]) return group[a] < group[b];
		// 아니라면 S[a+t..] 와 S[b+t..] 의 첫 t 글자를 비교한다
		return group[a + t] < group[b + t];
	}
};

vector<int> getSuffixArray(string s)
{
	int n = s.size();

	vector<int> perm(n);
	vector<int> group(n + 1);
	for (int index = 0; index < n; index++)
	{
		perm[index] = index;
		group[index] = s[index];
	}
	group[n] = -1;
	int t = 1;

	while (t < n)
	{
		Comparator compare(group, t);
		sort(perm.begin(), perm.end(), compare);

		t *= 2;
		if (t >= n) break;

		vector<int> newGroup(n + 1);
		newGroup[perm[0]] = 0;
		newGroup[n] = -1;
		for (int index = 1; index < n; index++)
		{
			if (compare(perm[index - 1], perm[index]))
				newGroup[perm[index]] = newGroup[perm[index-1]] + 1;
			else
				newGroup[perm[index]] = newGroup[perm[index - 1]];
		}
		group = newGroup;

	}
	return perm;
}



/*
// s 의 접미사 배열을 계산한다
vector<int> getSuffixArray(const string& s) {
	int n = s.size();

	// group[i] = 접미사들을 첫 t 글자를 기준으로 정렬했을 때,
	//            S[i..] 가 들어가는 그룹 번호.
	// t = 1 일 때는 정렬할 것 없이 S[i..] 의 첫 글자로 그룹 번호를 정해 줘도
	// 같은 효과가 있다.
	int t = 1;
	vector<int> group(n + 1);
	for (int i = 0; i < n; i++) group[i] = s[i];
	group[n] = -1;

	// 결과적으로 접미사 배열이 될 반환값. 이 배열을 lg(n) 번 정렬한다.
	vector<int> perm(n);
	for (int i = 0; i < n; i++) perm[i] = i;

	while (t < n) {
		// group[] 은 첫 t 글자를 기준으로 계산해 뒀다.
		// 첫 2t 글자를 기준으로 perm 을 다시 정렬한다
		Comparator compareUsing2T(group, t);
		sort(perm.begin(), perm.end(), compareUsing2T);

		// 2t 글자가 n 을 넘는다면 이제 접미사 배열 완성!
		t *= 2;
		if (t >= n) break;

		// 2t 글자를 기준으로 한 그룹 정보를 만든다
		vector<int> newGroup(n + 1);
		newGroup[n] = -1;
		newGroup[perm[0]] = 0;
		for (int i = 1; i < n; i++)
			if (compareUsing2T(perm[i - 1], perm[i]))
				newGroup[perm[i]] = newGroup[perm[i - 1]] + 1;
			else
				newGroup[perm[i]] = newGroup[perm[i - 1]];
		group = newGroup;
	}
	return perm;
}
*/

int maxSubstringLength(string s, int frequency)
{
	int ret = 0;

	if (frequency == 1)
		return s.size();

	vector<int> suffixList = getSuffixArray(s);
	for (int index = frequency - 1; index < s.size(); index++)
	{
		int length = commonPrefix(s, suffixList[index], suffixList[index - frequency + 1]);
		ret = max(ret, length);
	}


	return ret;
}


// Solve the problem(read the input file / and retrieves the result
void solveProblem(char *filename, bool isFile)
{
	//	gFpOutput = fopen("Test.txt", "w");
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

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);

	for (int i = 0; i < testCase; i++)
	{
		int frequency;
		char buf[4001];
		fscanf(fpInput, "%d\n", &frequency);
		fscanf(fpInput, "%s\n", buf);

		int ret = maxSubstringLength(string(buf), frequency);

		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	//	fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif

	return 0;
}