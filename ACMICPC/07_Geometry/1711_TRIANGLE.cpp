#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>


using namespace std;
FILE *fpInput;
FILE *fpOutput;
typedef long long int64;

int N;
vector<pair<int, int>> points;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	points = vector<pair<int, int>>(N);
	for (int i = 0; i < N; i++) {
		fscanf(fpInput, "%d %d", &points[i].first, &points[i].second);
	}
}

int gcd(int a, int b)
{
	if (b == 0) return a;
	return gcd(b, a%b);
}

pair<int, int> normalize(pair<int, int> input)
{
	if (input.first == 0) return make_pair(0, input.second/abs(input.second));
	if (input.second == 0) return make_pair(input.first / abs(input.first), 0);
	int gcdVal = gcd(abs(input.first), abs(input.second));
	return make_pair(input.first / gcdVal, input.second / gcdVal);
}

void shiftPoint(pair<int, int> &v)
{
	if (v.first == 0) v.second = abs(v.second);
	if (v.first < 0) {
		v.first *= (-1);
		v.second *= (-1);
	}
}

int64 findTriangleCount()
{
	int64 ret = 0;

	for (int p1 = 0; p1 < N; p1++) {
		pair<int, int> origin = points[p1];
		map<pair<int, int>, int> mPoints;
		for (int p2 = 0; p2 < N; p2++) {
			if (p1 == p2) continue;
			pair<int, int> v(points[p2].first - origin.first, points[p2].second - origin.second);
			v = normalize(v);
			shiftPoint(v);
			mPoints[v]++;
		}

		for (auto p : mPoints) {
			auto v = p.first;
			pair<int, int> ortho(-v.second, v.first);
			shiftPoint(ortho);
			if (mPoints.find(ortho) != mPoints.end())
				ret += (p.second*mPoints[ortho]);
		}
	}
	ret /= 2;
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
	while (testCase-- > 0) {
		readInputData();
		auto ret = findTriangleCount();
#ifdef _FILE_
		printf("%lld\n", ret);
#endif
		fprintf(fpOutput, "%lld\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
