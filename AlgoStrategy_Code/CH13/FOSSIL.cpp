#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#if 0
using namespace std;

struct Position
{
	double x; 
	double y;
};

vector<Position> hull1;
vector<Position> hull2;
vector<pair<Position, Position>> hullUpVertex;
vector<pair<Position, Position>> hullDownVertex;
//vector<pair<Position, Position>> hullUpVertex2;
//vector<pair<Position, Position>> hullDownVertex2;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;


void formVertexFromHull(vector<Position> hull, vector<pair<Position, Position>> &vertexUpList, vector<pair<Position, Position>> &vertexDownList)
{
	Position prevEdge;
	for (int index = 0; index < hull.size(); index++)
	{
		Position edge = hull[index];

		if (index > 0)
		{
			if (prevEdge.x <= edge.x)
				vertexDownList.push_back(make_pair(prevEdge, edge));
			else
				vertexUpList.push_back(make_pair(edge, prevEdge));
		}

		prevEdge = edge;
	}

	if (prevEdge.x <= hull[0].x)
		vertexDownList.push_back(make_pair(prevEdge, hull[0]));
	else
		vertexUpList.push_back(make_pair(hull[0], prevEdge));
}

bool betweenVertex(double x, pair<Position, Position> vertex)
{
	if (x >= vertex.first.x && x <= vertex.second.x)
		return true;

	return false;
}

double getYFromVertex(double x, pair<Position, Position> v)
{
	double a = (v.first.y - v.second.y) / (v.first.x - v.second.x);
	double b = v.first.y - a * v.first.x;
	double y = a * x + b;
	return y;
}

double getOverlapVertical(double x, vector<pair<Position, Position>> &vertexUpList, vector<pair<Position, Position>> &vertexDownList)
{
	double minUp = 1e20, maxDown = -1e20;

	vector<pair<Position, Position>>::iterator vIter;
	for (vIter = vertexUpList.begin(); vIter != vertexUpList.end(); vIter++)
	{
		pair<Position, Position> v = (*vIter);
		if (betweenVertex(x, v))
		{
			double matchY = getYFromVertex(x, v);
			minUp = min(minUp, matchY);
		}
	}

	for (vIter = vertexDownList.begin(); vIter != vertexDownList.end(); vIter++)
	{
		pair<Position, Position> v = (*vIter);
		if (betweenVertex(x, v))
		{
			double matchY = getYFromVertex(x, v);
			maxDown = max(maxDown, matchY);
		}
	}

	return minUp - maxDown;
}

double minMaxXFromHull(vector<Position> hull, bool bMinimum)
{
	double ret = -1e20;
	if (bMinimum)
		ret = 1e20;

	vector<Position>::iterator itr;
	for (itr = hull.begin(); itr != hull.end(); itr++)
	{
		Position pos = (*itr);
		if (bMinimum)
			ret = min(ret, pos.x);
		else
			ret = max(ret, pos.x);
	}
	return ret;
}

double getMaxVertical()
{
	// 메인 알고리즘
	double lo, high;
	lo = max(minMaxXFromHull(hull1, true), minMaxXFromHull(hull2, true));
	high = min(minMaxXFromHull(hull1, false), minMaxXFromHull(hull2, false));

	if (lo > high)
		return 0.0;

	for (int iter = 0; iter < 100; iter++)
	{
		double aab = (2 * lo + high) / 3.0;
		double abb = (lo + 2 * high) / 3.0;
		
		double verticalA = getOverlapVertical(aab, hullUpVertex, hullDownVertex);
		double verticalB = getOverlapVertical(abb, hullUpVertex, hullDownVertex);
		//printf("(%lf, %lf) (%lf, %lf)\n", aab, verticalA, abb, verticalB);
		if (verticalA > verticalB)
			high = abb;
		else
			lo = aab;
	}

	double finalX = (lo + high) / 2.0;
	double ret = getOverlapVertical(finalX, hullUpVertex, hullDownVertex);
	if (ret < 0)
		ret = 0.0;
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
		hull1.clear();
		hull2.clear();
		hullUpVertex.clear();
		hullDownVertex.clear();

		int n, m;
		fscanf(fpInput, "%d %d", &n, &m);

		for (int j = 0; j < n; j++)
		{
			Position pos;
			fscanf(fpInput, "%lf %lf", &pos.x, &pos.y);
			hull1.push_back(pos);
		}

		for (int j = 0; j < m; j++)
		{
			Position pos;
			fscanf(fpInput, "%lf %lf", &pos.x, &pos.y);
			hull2.push_back(pos);
		}

		formVertexFromHull(hull1, hullUpVertex, hullDownVertex);
		formVertexFromHull(hull2, hullUpVertex, hullDownVertex);
		double ret = getMaxVertical();
		fprintf(fpOutput, "%.8f", ret);
		if (isFile)
			printf("%.8f", ret);
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