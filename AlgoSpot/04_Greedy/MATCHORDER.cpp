#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

vector<int> russianRatings;
vector<int> koreaRatings;


void readInputData()
{
    russianRatings.clear();
    koreaRatings.clear();

    int members;
    fscanf(fpInput, "%d", &members);
    russianRatings = vector<int>(members);
    koreaRatings = vector<int>(members);

    for (int index = 0; index < members; index++)
        fscanf(fpInput, "%d", &russianRatings[index]);

    for (int index = 0; index < members; index++)
        fscanf(fpInput, "%d", &koreaRatings[index]);

    sort(russianRatings.begin(), russianRatings.end());
    sort(koreaRatings.begin(), koreaRatings.end());
}

int findMaxWins()
{
    int ret = 0;

    while (!russianRatings.empty()) {
        int rating = *russianRatings.begin();

        if (koreaRatings[koreaRatings.size() - 1] < rating) {
            koreaRatings.erase(koreaRatings.begin());
        }
        else {
            ret++;
            koreaRatings.erase(lower_bound(koreaRatings.begin(), koreaRatings.end(), rating));
        }
        russianRatings.erase(russianRatings.begin());
    }

    return ret;
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
    while (testCase > 0) {
        readInputData();

        int ret = findMaxWins();
        if (isFile)
            printf("%d\n", ret);
        fprintf(fpOutput, "%d\n", ret);

        testCase--;
    }

    fclose(fpInput);
    fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE
    solveProblem(argv[1], true);
#else
    solveProblem("", false);
#endif

    return 0;
}
