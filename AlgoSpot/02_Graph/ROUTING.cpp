#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int N, M;
vector<vector<pair<int, double>>> adj;

void readInputData()
{
  fscanf(fpInput, "%d %d", &N, &M);

  adj = vector<vector<pair<int, double>>>(N, vector<pair<int, double>>());

  for (int index = 0; index < M; index++) {
    int v1, v2;
    double cost;
    fscanf(fpInput, "%d %d %lf", &v1, &v2, &cost);

    adj[v1].push_back(make_pair(v2, cost));
    adj[v2].push_back(make_pair(v1, cost));
  }
}

double minNoiseCost()
{
  vector<double> dist;
  priority_queue < pair<double, int>, vector<pair<double, int>>, 
            std::greater<pair<double, int>>> q;

  dist = vector<double>(N, numeric_limits<double>::max());
  dist[0] = 1;
  q.push(make_pair(1.0f, 0));
  while (!q.empty()) {
    int here = q.top().second;
    double cost = q.top().first;
    q.pop();

    if (dist[here] < cost)
      continue;

    for (auto there : adj[here]) {
      double nextCost = cost * there.second;

      if (nextCost < dist[there.first]) {
        dist[there.first] = nextCost;
        q.push(make_pair(nextCost, there.first));
      }
    }
  }

  return dist[N-1];
}

void solveProblem(const char *fileName, bool isFile)
{
  fpInput = stdin;
  fpOutput = stdout;
  if (isFile) {
    fpInput = fopen(fileName, "r");
    string outputFileName = string(fileName);
    outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
    fpOutput = fopen(outputFileName.c_str(), "w");
  }

  int testCase = 0;

  fscanf(fpInput, "%d", &testCase);
  while (testCase > 0)
  {
    readInputData();

    double ret = minNoiseCost();
    if (isFile)
      printf("%.10f\n", ret);
    fprintf(fpOutput, "%.10f\n", ret);

    testCase--;
  }

  fclose(fpInput);
  fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE_
  solveProblem(argv[1], true);
#else
  solveProblem("", false);
#endif


  return 0;
}
