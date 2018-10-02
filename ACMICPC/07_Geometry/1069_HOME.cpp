#include <stdio.h>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <set>


using namespace std;

FILE *fpInput;
FILE *fpOutput;



int x, y, D, T;

void readInputData()
{
	fscanf(fpInput, "%d %d %d %d", &x, &y, &D, &T);
}

double minPathToHome()
{
	double ret = 0;

	double dist = sqrt(x*x + y*y);
	ret = dist;

	if (D > T) {
		double remain = ((int)dist%D) + (dist - (int)dist);
		int divide = ((int)dist / D);
		double before = divide * T + remain;
		double after = divide * T + T;
		if (divide == 0) {
			after = min(T+D-remain, (double)2*T);
		}
		ret = min(ret, before);
		ret = min(ret, after);
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
		auto ret = minPathToHome();
#ifdef _FILE_
		printf("%.11f\n", ret);
#endif
		fprintf(fpOutput, "%.11f\n", ret);
		testCase--;
	}
}
