#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
#define MAX_NUM_VALUE	10000

FILE *fpInput;
FILE *fpOutput;
typedef long long int64;

const int MAX_P = 100000;
vector<int> primes;
vector<int> minPrimes;
int64 A, B;
int N;

void readInputData()
{
	fscanf(fpInput, "%lld %lld %d", &A, &B, &N);
}


void precalcPrimes()
{
	minPrimes = vector<int>(MAX_P+1, 0);
	for (int i = 1; i < MAX_P; i++)
		minPrimes[i] = i;

	for (int i = 2; i < MAX_P; ++i) {
		if (minPrimes[i] == i) {
			primes.push_back(i);
			for (int64 j = (int64)i * i; j < MAX_P; j += i)
				minPrimes[j] = i;
		}
	}
}


int64 noUnionCount(int64 start, int64 end, int n)
{
	int64 ret = end - start + 1;
	if (n == 1)
		return ret;

	vector<int> nPrimes;
	int temp = n;
	for (auto p : primes) {
		if (temp % p == 0) {
			nPrimes.push_back(p);
			while ((temp % p) == 0)
				temp /= p;
		}
		if (p > temp)
			break;
	}
	if (temp > 1) nPrimes.push_back(temp);

	for (int mask = 1; mask < (1 << (int)nPrimes.size()); ++mask) {
		int64 d = 1, sgn = -1;

		for (int i = 0; i < nPrimes.size(); ++i){
			if (mask & (1 << i)) {
				d *= nPrimes[i];
				sgn *= -1;
			}
		}

		int64 multiples = (end / d) - (start / d);
		if (start % d == 0)
			multiples++;

		ret -= sgn * multiples;
	}

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
	int index = 1;
	precalcPrimes();
	fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = noUnionCount(A, B, N);
#ifdef _FILE_
		printf("Case #%d: %lld\n", index, ret);
#endif
		fprintf(fpOutput, "Case #%d: %lld\n", index, ret);

		index++;
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
