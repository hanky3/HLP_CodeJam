#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
typedef long long int64;
FILE *fpInput;
FILE *fpOutput;

#define MAX_PASSWORD_LEN		5001
#define MOD										1000000

string password;
int64 cache[MAX_PASSWORD_LEN];

void readInputData()
{
	memset(cache, -1, sizeof(cache));
	char buf[MAX_PASSWORD_LEN];
	fscanf(fpInput, "%s", buf);
	password = buf;
}

int64 decryptCount(int pos)
{
	int64 &ret = cache[pos];
	if (ret != -1)	return ret;
	if (pos >= password.length())
		return 1;

	// single code
	ret = 0;
	int code = password[pos] - '0';
	if (code == 0)
		return ret;

	ret += decryptCount(pos + 1);
	ret %= MOD;
	// multi-code
	if (password.length() - pos >= 2) {
		string multiCode = password.substr(pos, 2);
		sscanf(multiCode.c_str(), "%d", &code);
		if (code >= 10 && code <= 26) {
			ret += decryptCount(pos + 2);
			ret %= MOD;
		}
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
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = decryptCount(0);
#ifdef _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}