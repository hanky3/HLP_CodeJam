#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;
typedef long long int64;

FILE *fpInput;
FILE *fpOutput;

int N, K;
vector<int> deviceList;
vector<vector<int>> usages;

void readInputData()
{
	fscanf(fpInput, "%d %d\n", &N, &K);
	deviceList = vector<int>(K);
	usages = vector<vector<int>>(K);
	for (int i = 0; i < K; i++) {
		fscanf(fpInput, "%d", &deviceList[i]);
		deviceList[i]--;
		usages[deviceList[i]].push_back(i);
	}

	for (int i = 0; i < K; i++) {
		usages[i].push_back(K);
	}
}

int minTabPlugin()
{
	int ret = 0;

	//set<DeviceUsage> deviceUsages;
	vector<int> deviceUsages;
	for (auto device : deviceList) {
		usages[device].erase(usages[device].begin());

		auto itr = find(deviceUsages.begin(), deviceUsages.end(), device);
		if (itr != deviceUsages.end()) {
			continue;
		}

		if (deviceUsages.size() < N) {
			deviceUsages.push_back(device);
			continue;
		}
		sort(deviceUsages.begin(), deviceUsages.end(), [&](int a, int b) {
			return (usages[a][0] > usages[b][0]);
		});
		deviceUsages.erase(deviceUsages.begin());
		deviceUsages.push_back(device);
		ret++;
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
	while (testCase > 0) {
		readInputData();
		auto ret = minTabPlugin();

#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}
