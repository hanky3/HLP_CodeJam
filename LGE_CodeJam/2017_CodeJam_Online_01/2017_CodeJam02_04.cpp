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

#if 0
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
#define MOD						1000000007
#define NATURAL_NUM_CNT	5

vector<int> naturalNumList;
vector<int> naturalSums;

void generateNumVectors(vector<int> &num, int input)
{
	while (input > 0)
	{
		num.push_back(input % 10);
		input /= 10;
	}
}


bool mergeNumbers(vector<int> &num1, vector<int> &sums)
{
	if (sums.size() < num1.size())
		sums.resize(num1.size());

	int remains = 0;
	for (int i = 0; i < num1.size(); i++)
	{
		sums[i] += num1[i];
		sums[i] += remains;

		remains = sums[i] / 10;
		sums[i] %= 10;
	}

	int next = num1.size();
	while (remains > 0)
	{
		if (next >= sums.size()) {
			sums.push_back(remains);
			break;
		}

		sums[next] += remains;
		remains = sums[next] / 10;
		sums[next] %= 10;
		next++;
	}
	return true;
}

bool isSameDigitNum(int num, int digit, vector<int> &nums)
{
	if (nums.size() != digit)
		return false;

	for (auto n : nums)
	{
		if (n != num)
			return false;
	}

	return true;
}


int getNumber(vector<int> &num, int length)
{
	int ret = 0;

	int start = num.size() - length;
	if (start < 0)
		start = 0;
	int digit = 1;
	for (int index = start; index < num.size(); index++)
	{
		ret += num[index] * digit;
		digit *= 10;
	}

	return ret;
}

void updateNumVector(vector<int> &num, int length, int inputValue)
{
	int index = num.size() - length;
	
	if (index < 0)
		return;

	while (true)
	{
		num[index++] = inputValue % 10;
		inputValue /= 10;

		if (inputValue <= 0)
			break;
	}

	num.erase(num.begin() + index, num.end());
}

int findNaturalNumIndex(int num, int digit, int psum)
{
	vector<int> sameDigitNums(digit, num);
	vector<int> sumNums, psumNums;
	generateNumVectors(sumNums, naturalSums[4]);
	generateNumVectors(psumNums, psum);

	vector<int> counter;
	int separateLength = sumNums.size();
	int index = sameDigitNums.size() - separateLength;
	for (; index >= 0; index--)
	{
		int currentDigitSize = sameDigitNums.size();
		int highBitNum = getNumber(sameDigitNums, sameDigitNums.size() - index);

		int count = 0;
		while (highBitNum > 0)
		{
			highBitNum -= naturalSums[4];
			count++;
		}

		if (highBitNum < 0)
		{
			count--;
			highBitNum += naturalSums[4];
		}

		counter.push_back(count);
		updateNumVector(sameDigitNums, sameDigitNums.size() - index, highBitNum);
		separateLength = sumNums.size();
	}

	unsigned long long ret = 0;
	unsigned long long prevDigit = 1;
	for (int index = counter.size() - 1; index >= 0; index--)
	{
		unsigned long long digitNum = (counter[index] * prevDigit) % MOD;
		digitNum *= 5;
		digitNum %= MOD;

		ret += digitNum;
		ret %= MOD;

		prevDigit = (prevDigit * 10) % MOD;
	}

	return ret;
}

int prevDigitNumcache[10];
int makeSameDigitNum(int num, int digit, int &sequence)
{
	unsigned long long newNum = 0;
	sequence = 0;
	
	newNum = (prevDigitNumcache[num] * 10) % naturalSums[4];
	newNum += num;
	newNum %= naturalSums[4];
	/*
	unsigned long long next = num;
	for (int d = 0; d < digit; d++)
	{
		newNum += next;
		newNum %= naturalSums[4];

		next = (next * 10) % naturalSums[4];
	}
	*/
	prevDigitNumcache[num] = newNum;

	return newNum;
}

int findFirstSameDigit(int &num, int &digit)
{
	int next = 0;
	
	digit = 2;
	while (true)
	{
		for (num = 1; num <= 9; num++)
		{
			int seq = 0;
			int nextSameDigit = makeSameDigitNum(num, digit, seq);
			int sum = 0;
			
			if (nextSameDigit == 0)
				return findNaturalNumIndex(num, digit, 0) - 1;

			auto itr = std::find(naturalSums.begin(), naturalSums.end(), nextSameDigit);
			if (itr != naturalSums.end())
			{
				int remains = itr - naturalSums.begin();
				return findNaturalNumIndex(num, digit, *itr) + remains;
			}
		}
		digit++;
	}

	return -1;
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
		naturalNumList = vector<int>(NATURAL_NUM_CNT);
		naturalSums = vector<int>(NATURAL_NUM_CNT);

		for (int i = 0; i < 10; i++)
			prevDigitNumcache[i] = i;

		for (int index = 0; index < NATURAL_NUM_CNT; index++) {
			fscanf(fpInput, "%d", &naturalNumList[index]);
			
			naturalSums[index] += naturalNumList[index];
			if (index > 0)
				naturalSums[index] += naturalSums[index - 1];
		}


		int num, digit;
		int ret = findFirstSameDigit(num, digit);
		fprintf(fpOutput, "%d %d(%d)\n", ret, num, digit);
		if (isFile)
			printf("%d %d(%d)\n", ret, num, digit);
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
#if 1
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif