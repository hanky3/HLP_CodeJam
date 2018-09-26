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
#define MAX_SIZE			151
typedef long long int64;

string number;
vector<int> numbers;

void normalize(vector<int> &num)
{
	num.push_back(0);
	for (int i = 0; i < num.size() - 1; ++i) {
		if (num[i] < 0) {
			int borrow = abs(num[i] + 9) / 10;
			num[i + 1] -= borrow;
			num[i] += borrow * 10;
		}
		else {
			num[i + 1] += num[i] / 10;
			num[i] %= 10;
		}
	}
	while (num.size() > 1 && num.back() == 0)
		num.pop_back();
}

void readInputData()
{
	char buf[MAX_SIZE];
	fscanf(fpInput, "%s", buf);
	number = buf;
	numbers.clear();
	for (int pos = number.size() - 1; pos >= 0; pos--) {
		numbers.push_back(number[pos] - '0');
	}
	normalize(numbers);
}

vector<int> multiply(const vector<int>& a, const vector<int>& b)
{
	vector<int> c(a.size() + b.size() + 1, 0);
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < b.size(); j++) {
			c[i + j] += a[i] * b[j];
		}
	}
	normalize(c);
	return c;
}

vector<int> add(const vector<int>& a, const vector<int>& b)
{
	vector<int> c(max(a.size(), b.size()), 0);
	for (int i = 0; i < c.size(); i++) {
		if (i < a.size())	c[i] += a[i];
		if (i < b.size())	c[i] += b[i];
	}
	normalize(c);
	return c;
}

vector<int> divide(const vector<int>& a, int n)
{
	vector<int> c(a.size(), 0);
	int added = 0;
	for (int i = c.size()-1; i >= 0; i--) {
		added += a[i];
		c[i] = added / n;
		added = (10 * (added % n));
	}
	normalize(c);
	return c;
}

int compare(const vector<int>& a, const vector<int>& b)
{
	if (a.size() != b.size())
		return (a.size() - b.size());

	for (int i = a.size()-1; i >= 0; i--) {
		if (a[i] > b[i]) return 1;
		if (a[i] < b[i]) return -1;
	}
	return 0;
}

vector<int> findCubeValue()
{
	vector<int> ret;
	vector<int> lo, hi, mid;

	bool allZero = true;
	for (auto n : numbers) {
		if (n != 0) {
			allZero = false;
			break;
		}
	}
	if (allZero) {
		ret = vector<int>(11, 0);
		return ret;
	}

	lo = vector<int>(1, 0);
	hi = vector<int>(numbers.size()/3+1, 9);
	for (int i = 0; i < 10; i++) {
		lo.insert(lo.begin(), 0);
		hi.insert(hi.begin(), 0);
	}

	for (int iter = 0; iter <= 300; iter++) {
		mid = add(lo, hi);
		mid = divide(mid, 2);

		vector<int> multi = multiply(mid, mid);
		multi = multiply(multi, mid);
		bool remains = false;
		for (int i = 0; i < 30; i++) {
			if (multi[i] != 0) {
				remains = true;
				break;
			}
		}
		auto cmp = compare(vector<int>(multi.begin() + 30, multi.end()), numbers);
		if ((cmp < 0) || (cmp == 0 && !remains))
			lo = mid;
		else
			hi = mid;
	}
	return lo;
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
		auto ret = findCubeValue();
		string retStr;
		for (auto n : ret) {
			retStr.push_back(n+'0');
		}
		retStr.insert(retStr.begin() + 10, '.');
		reverse(retStr.begin(), retStr.end());
#ifdef _FILE_
		printf("%s\n", retStr.c_str());
#endif
		fprintf(fpOutput, "%s\n", retStr.c_str());
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
