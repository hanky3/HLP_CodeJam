//============================================================================
// Name        : lis2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <cstring>
#define MAX_N	(500)
//#define _FILE_IO_
using namespace std;

FILE* fpIn;
FILE* fpOut;
vector<int> v;
int cache[MAX_N];

int LIS(unsigned int start)
{
	if (start == v.size() - 1)
		return 1;
	vector<int> tempV;
	
	int& ret = cache[start];

	if (ret != -1)
		return ret;
	
	ret = 1;
	for (unsigned int jump = 1; start + jump <= v.size() - 1; jump++)
	{
		if (v[start] < v[start + jump])
		{
			int temp = LIS(start + jump) + 1;
			tempV.push_back(temp);
		}
	}

	if (tempV.empty() == false)
		ret = *(max_element(tempV.begin(), tempV.end()));
//	ret = result;

	return ret;
}

int main() {
	int testcase = 0;
#ifdef _FILE_
	fpIn = fopen("input.in", "r");
	fpOut = fopen("output.txt", "w");
#else
	fpIn = stdin;
	fpOut = stdout;
#endif
	fscanf(fpIn, "%d", &testcase);
	while (testcase-- > 0)
	{
		int nos = 0; // Number of Sequence
		vector<int> resultV;
		memset(cache, -1, sizeof(cache));
		fscanf(fpIn, "%d", &nos);
		// Input
		v.push_back(0);
		// Start index is 1
		for (int i = 0; i < nos; i++)
		{
			int temp = 0;
			fscanf(fpIn, "%d", &temp);
			v.push_back(temp);
		}
		for (unsigned int start = 1; start <= v.size() - 1; start++)
			resultV.push_back(LIS(start));

		int ret = *max_element(resultV.begin(), resultV.end());
		fprintf(fpOut, "%d\n", ret);
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		v.clear();
	}
#ifdef _FILE_IO_
	fclose(fpIn);
	fclose(fpOut);
#endif
	return 0;
}