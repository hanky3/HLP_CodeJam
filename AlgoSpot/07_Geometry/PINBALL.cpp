#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

using namespace std;

const double PI = 2.0 * acos(0.0);
class vector2
{
public:
	vector2(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
	bool operator == (const vector2 &rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator < (const vector2 &rhs) const {
		return x != rhs.x ? x < rhs.x : y < rhs.y;
	}
	vector2 operator + (const vector2 &rhs) const {
		return vector2(x + rhs.x, y + rhs.y);
	}
	vector2 operator - (const vector2 &rhs) const {
		return vector2(x - rhs.x, y - rhs.y);
	}
	vector2 operator * (double rhs) const {
		return vector2(x*rhs, y*rhs);
	}
	double norm() const { return hypot(x, y); }
	vector2 normalize() const { return vector2(x / norm(), y / norm()); }
	double polar() const { return fmod(atan2(y, x) + 2 * PI, 2 * PI); }
	double dot(const vector2 &rhs) const { return x * rhs.x + y * rhs.y; }
	double cross(const vector2 &rhs) const { return x * rhs.y - y * rhs.x; }
	vector2 project(const vector2& rhs) const {
		vector2 r = rhs.normalize();
		return r * r.dot(*this);
	}

	double x, y;
};


FILE *fpInput = NULL;
FILE *fpOutput = NULL;

struct PinballInfo
{
	int x, y;
	int dx, dy;
};

struct ObstacleInfo
{
	int x, y;
	int radius;
};

PinballInfo pinBallInfo;
vector<ObstacleInfo> obstacleList;

const double EPILSON = 1e-9;
const double INFINI = 1e200;
vector<double> solve2(double a, double b, double c)
{
	double D = b*b - 4 * a*c;

	if (D < -EPILSON)
		return vector<double>();
	if (D < EPILSON)
		return vector<double>(1, -b / (2 * a));

	vector<double> ret;
	ret.push_back((-b - sqrt(D)) / (2 * a));
	ret.push_back((-b + sqrt(D)) / (2 * a));
	return ret;
}

bool hitCircle(vector2 here, vector2 dir, vector2 center, int radius, double &output)
{
	double a = dir.dot(dir);
	double b = 2 * dir.dot(here - center);
	double c = center.dot(center) + here.dot(here) - (2 * (center.dot(here))) - (radius*radius);
	vector<double> solve = solve2(a, b, c);
	if (solve.empty() || solve[0] < EPILSON)
		return false;

	output = solve[0];
	return true;
}

int findHitObstacle(vector2 pos, vector2 dir, vector2 &newPos)
{
	int obstacleSize = obstacleList.size();

	int obstacleIdx = -1;
	double minDt = INFINI;
	for (int i = 0; i < obstacleSize; i++)
	{
		ObstacleInfo obs = obstacleList[i];
		vector2 center(obs.x, obs.y);
		double dt = 0;
		if (hitCircle(pos, dir, center, obs.radius + 1, dt))
		{
			if (dt < minDt)
			{
				obstacleIdx = i;
				minDt = dt;
				newPos = pos + dir*dt;
			}
		}
	}

	return obstacleIdx;
}

vector2 nextDirectionVec(int obstacleId, vector2 crossVec, vector2 dir)
{
	vector2 rdir = vector2(-dir.x, -dir.y);
	vector2 center = vector2(obstacleList[obstacleId].x, obstacleList[obstacleId].y);

	vector2 projectV = rdir.project(crossVec - center);
	vector2 nextDir = (rdir + (projectV - rdir) * 2).normalize();

	return nextDir;
}

vector<int> hitObstacleList()
{
	vector<int> obsList;
	int hitCount = 0;

	vector2 originPos(pinBallInfo.x, pinBallInfo.y);
	vector2 dir(pinBallInfo.dx, pinBallInfo.dy);
	while (true)
	{
		if (hitCount >= 100)
			break;

		vector2 newPosVec;
		int obstacleId = findHitObstacle(originPos, dir, newPosVec);

		if (obstacleId < 0)
			break;

		obsList.push_back(obstacleId);
		dir = nextDirectionVec(obstacleId, newPosVec, dir);
		originPos = newPosVec;

		hitCount++;
	}

	return obsList;
}


void solveProblem(char *filename, bool isFile)
{
	//	gFpOutput = fopen("Test.txt", "w");

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

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);

	for (int i = 0; i < testCase; i++)
	{
		int obstacleCnt = 0;
		fscanf(fpInput, "%d %d %d %d %d\n", &pinBallInfo.x, &pinBallInfo.y, &pinBallInfo.dx, &pinBallInfo.dy, &obstacleCnt);

		obstacleList.clear();
		obstacleList = vector<ObstacleInfo>(obstacleCnt);

		for (int j = 0; j < obstacleCnt; j++)
		{
			fscanf(fpInput, "%d %d %d", &obstacleList[j].x, &obstacleList[j].y, &obstacleList[j].radius);
		}

		vector<int> ret = hitObstacleList();
		vector<int>::iterator itr;
		for (itr = ret.begin(); itr != ret.end(); itr++) {
			fprintf(fpOutput, "%d ", (*itr));
			if (isFile)
				printf("%d ", (*itr));
		}

		fprintf(fpOutput, "\n");
		if (isFile)
			printf("\n");

	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	//	fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif

	return 0;
}