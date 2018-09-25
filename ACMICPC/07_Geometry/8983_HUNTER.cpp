#include <stdio.h>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <set>

using namespace std;
#define MAX_N 1000000000

FILE *fpInput;
FILE *fpOutput;

struct Point {
	int x;
	int y;
	int index;

	Point() {}
	Point(int x, int y) : x(x), y(y) {}

	bool operator<(const Point &p) const {
		if (y == p.y) return x < p.x;
		return y < p.y;
	}
};

int M, N, L;
vector<Point> animalPositions;
vector<Point> gunPositions;

void readInputData()
{
	fscanf(fpInput, "%d %d %d\n", &M, &N, &L);
	gunPositions = vector<Point>(M);
	animalPositions = vector<Point>(N);
	for (int n = 0; n < M; n++) {
		fscanf(fpInput, "%d", &gunPositions[n].x);
		gunPositions[n].y = 0;
	}
	for (int n = 0; n < N; n++) {
		fscanf(fpInput, "%d %d", &animalPositions[n].x, &animalPositions[n].y);
	}

	sort(gunPositions.begin(), gunPositions.end(), [](Point &p1, Point &p2) {
		return p1.x < p2.x;
	});
	sort(animalPositions.begin(), animalPositions.end(), [](Point &p1, Point &p2) {
		if (p1.x == p2.x) return p1.y < p2.y;
		return p1.x < p2.x;
	});
}

int animalInRange()
{
	int ret = 0;
	int start = 0, maxHeight;

	for (auto animalPos : animalPositions) {
		int currX = animalPos.x;
		maxHeight = L - abs(gunPositions[start].x - animalPos.x);
		while (start < M-1) {
			int nextHeight = L - abs(gunPositions[start+1].x - animalPos.x);
			if (nextHeight >= maxHeight) {
				maxHeight = nextHeight;
				start++;
				continue;
			}
			break;
		}

		if (maxHeight <= 0) continue;
		if (animalPos.y <= maxHeight) {
			ret++;
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
	while (testCase > 0) {
		readInputData();
		auto ret = animalInRange();
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
