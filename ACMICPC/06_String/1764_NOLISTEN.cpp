#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;
typedef long long int64;
FILE *fpInput;
FILE *fpOutput;

#define MAX_BUF_SIZE		1000001

int N, M;
vector<string> strList1;
vector<string> strList2;


void readInputData()
{
	char buf[21];
	fscanf(fpInput, "%d %d", &N, &M);
	strList1 = vector<string>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%s", buf);
		strList1[i] = buf;
	}
	sort(strList1.begin(), strList1.end());

	strList2 = vector<string>(M);
	for (int i = 0; i < M; i++) {
		fscanf(fpInput, "%s", buf);
		strList2[i] = buf;
	}
	sort(strList2.begin(), strList2.end());
}

vector<string> overlap(vector<string> &list1, vector<string> &list2)
{
	vector<string> ret;

	auto itr1 = list1.begin();
	auto itr2 = list2.begin();

	while (itr1 != list1.end() && itr2 != list2.end()) {
		auto str1 = *itr1;
		auto str2 = *itr2;
		if (str1 == str2) {
			ret.push_back(*itr1);
			itr1++; itr2++;
			continue;
		}
		else if (str1 < str2) {
			itr1++;
		}
		else {
			itr2++;
		}
	}

	return ret;
}

void generateOutput(vector<string> &output)
{
#ifdef _FILE_
	printf("%d\n", output.size());
#endif
	fprintf(fpOutput, "%d\n", output.size());

	for (auto str : output) {
#ifdef _FILE_
		printf("%s\n", str.c_str());
#endif
		fprintf(fpOutput, "%s\n", str.c_str());
	}
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
		auto ret = overlap(strList1, strList2);
		generateOutput(ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

