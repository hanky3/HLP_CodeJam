#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#if 0
using namespace std;

FILE *fpInput;
FILE *fpOutput;

struct TurretInfo {
	int x;
	int y;
	int radius;

	bool operator==(const TurretInfo &rhs) {
		if (this->x == rhs.x && this->y == rhs.y && this->radius == rhs.radius)
			return true;
		return false;
	}
};

TurretInfo turret1, turret2;

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &turret1.x, &turret1.y, &turret1.radius);
	fscanf(fpInput, "%d %d %d", &turret2.x, &turret2.y, &turret2.radius);
}

double getDistance(TurretInfo &a, TurretInfo &b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

void swapTurret(TurretInfo &a, TurretInfo &b)
{
	swap(a.x, b.x);
	swap(a.y, b.y);
	swap(a.radius, b.radius);
}

int getPositionCount()
{
	double distance = getDistance(turret1, turret2);
	if (turret1 == turret2)
		return -1;

	if (turret1.radius < turret2.radius)
		swapTurret(turret1, turret2);

	if (distance + turret2.radius <= turret1.radius) {
		if (abs((distance + turret2.radius) - turret1.radius) <= 1e-10)
			return 1;
		return 0;
	}
	if (abs(distance - (turret2.radius + turret1.radius)) <= 1e-10)
		return 1;
	if (distance > (double)(turret1.radius + turret2.radius))
		return 0;

	return 2;
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
		auto ret = getPositionCount();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
#endif