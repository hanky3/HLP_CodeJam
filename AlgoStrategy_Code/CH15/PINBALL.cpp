#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#if 0
using namespace std;

const double PI = 2.0 * acos(0.0);

struct PinballInfo
{
	int x;
	int y;
	int dx;
	int dy;
};

struct ObstacleInfo
{
	int x;
	int y;
	int radius;
};



FILE *fpInput = NULL;
FILE *fpOutput = NULL;

class vector2
{
public :
	vector2(double x_ = 0, double y_ = 0) : x(x_), y(y_)
	{}

	bool operator == (const vector2& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator < (const vector2& rhs) const
	{
		return x != rhs.x ? x < rhs.x : y < rhs.y;
	}

	vector2 operator + (const vector2& rhs) const
	{
		return vector2(x + rhs.x, y + rhs.y);
	}

	vector2 operator - (const vector2& rhs) const
	{
		return vector2(x - rhs.x, y - rhs.y);
	}

	vector2 operator * (double rhs) const
	{
		return vector2(x * rhs, y * rhs);
	}
	
	double norm() const {
		return hypot(x, y);
	}

	vector2 normalize() const {
		return vector2(x / norm(), y / norm());
	}

	double polar() const
	{
		return fmod(atan2(y, x) + 2 * PI, 2 * PI);
	}

	double dot(const vector2 &rhs)
	{
		return x*rhs.x + y*rhs.y;
	}

	double cross(const vector2& rhs) const
	{
		return x*rhs.y - rhs.x*y;
	}

	vector2 project(const vector2& rhs) const
	{
		vector2 r = rhs.normalize();
		return r * r.dot(*this);
	}

	double x, y;
	//fmod
};

PinballInfo pinBallInfo;
vector2 ballVector;
vector<ObstacleInfo> obstacleList;

vector2 perpendicularFoot(vector2 p, vector2 a, vector2 b)
{
	return a + (p - a).project(b - a);
}

double pointToLine(vector2 p, vector2 a, vector2 b)
{
	return (p - perpendicularFoot(p, a, b)).norm();
}

bool isVectorInObstacle(vector2 pos, vector2 dir, int obstacleIndex)
{
	vector2 forwardPos = pos + dir*1000;
	vector2 obstacleVec(obstacleList[obstacleIndex].x, obstacleList[obstacleIndex].y);

	double pointFromObstacle = pointToLine(obstacleVec, pos, forwardPos);

	if (pointFromObstacle <= obstacleList[obstacleIndex].radius)
		return true;
	else
		return false;

}

vector<int> hitPinBallList()
{
	// 메인 알고리즘
	vector<int> ret;

	return ret;
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
		int N;
		fscanf(fpInput, "%d %d %d %d %d", &pinBallInfo.x, &pinBallInfo.y, &pinBallInfo.dx, &pinBallInfo.dy, &N);
		ballVector = vector2(pinBallInfo.x, pinBallInfo.y);

		obstacleList.clear();
		obstacleList = vector<ObstacleInfo>(N);
		for (int j = 0; j < N; j++)
		{
			fscanf(fpInput, "%d %d %d", &obstacleList[j].x, &obstacleList[j].y, &obstacleList[j].radius);
		}

		vector2 dir(pinBallInfo.dx, pinBallInfo.dy);
		bool isVectorIn = isVectorInObstacle(ballVector, dir, 0);
//		printf("%lf\n", test.cross(test2));
//		double ret = getMaxVertical();

		fprintf(fpOutput, "\n");
		if (isFile)
			printf("\n");
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
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif 