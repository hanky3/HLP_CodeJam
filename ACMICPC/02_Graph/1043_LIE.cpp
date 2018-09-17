#include <stdio.h>
#include <string.h>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

FILE *fpInput;
FILE *fpOutput;


int N, M;
vector<vector<int>> adj;
vector<int> knowTruth;
vector<vector<int>> partyPeople;

void readInputData()
{
	fscanf(fpInput, "%d %d\n", &N, &M);
	
	int count;
	fscanf(fpInput, "%d", &count);
	knowTruth = vector<int>(N);
	for (int i = 0; i < count; i++) {
		int pos;
		fscanf(fpInput, "%d", &pos);
		knowTruth[pos-1]++;
	}
	
	partyPeople = vector<vector<int>>(M);
	for (int i = 0; i < M; i++) {
		fscanf(fpInput, "%d", &count);
		partyPeople[i] = vector<int>(count);
		for (int j = 0; j < count; j++) {
			fscanf(fpInput, "%d", &partyPeople[i][j]);
			partyPeople[i][j]--;
		}
	}
}

int maxPartyCount()
{
	int ret = 0;

	map<string, int> visited;
	queue<tuple<int, int, string>> q;

	string initListening(N, 'u');
	q.push(make_tuple( 0,0, initListening ));

	while (!q.empty()) {
		int party = std::get<0>(q.front());
		int selected = std::get<1>(q.front());
		string listening = std::get<2>(q.front());
		q.pop();

		int remains = (M - party);
		if (selected + remains <= ret)
			continue;

		if (party == M) {
			ret = max(ret, selected);
			continue;
		}

		// truth people
		bool tellTruth = false;
		for (auto p : partyPeople[party]) {
			if (knowTruth[p]) {
				tellTruth = true;
				break;
			}
		}

		bool satisfied[2] = { true, true };
		string nextListening[2] = { listening, listening };
		for (auto p : partyPeople[party]) {
			if (listening[p] == 't') {
				satisfied[0] = false;
			}
			else if (listening[p] == 'l') {
				satisfied[1] = false;
			}
			nextListening[0][p] = 'l';
			nextListening[1][p] = 't';
		}

		if (!tellTruth && satisfied[0]) {
			q.push(make_tuple(party+1, selected+1, nextListening[0]));
		}
		if (satisfied[1]) {
			q.push(make_tuple( party + 1, selected, nextListening[1] ));
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
		auto ret = maxPartyCount();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}

