#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
using namespace std;

#if 0
const int INF = 0x0fffffff;
FILE *fpInput = NULL;
FILE *fpOutput = NULL;

const int MAX_SEMESTER = 11;
const int MAX_TAKEN = 0x1000;

vector<int> semesterClassInfo;
vector<int> preClassInfo;
int maxClasses;
int totalClasses;
int toCompleteClasses;
int totalSemester;
int cache[MAX_SEMESTER + 1][MAX_TAKEN];

int getBitCount(int bitInfo)
{
#if WIN32
	return __popcnt(bitInfo);
#else
	return __builtin_popcount(bitInfo);
#endif
}

bool isClassAvailable(int newClasses, int classTaken)
{
	int currClass = newClasses;

	if (currClass == 0)
		return false;

	while (currClass)
	{
		unsigned long index;
#if WIN32
		_BitScanForward(&index, currClass);
#else
		index = __builtin_ctz(currClass);
#endif

		if (preClassInfo[index] && ((preClassInfo[index] & classTaken) != preClassInfo[index]))
			return false;

		currClass &= (currClass-1);
	}

	return true;
}

int minSemesterToGraduate(int currSemester, int taken)
{
	int &ret = cache[currSemester][taken];
	// 메모이제이션 된 경우
	if (ret > 0) return ret;

	// 기저사례
	if (getBitCount(taken) >= toCompleteClasses)
		return 0;

	if (currSemester >= totalSemester)
		return INF;


	ret = INF;
	int currClassInfo = semesterClassInfo[currSemester];
	for (int newClass = currClassInfo&~taken; newClass; newClass = ((newClass - 1) & currClassInfo))
	{
		// 현재신청 과목이 학기 최대 수강과목보다 큰 경우
		if (getBitCount(newClass) > maxClasses) continue;
		// 현재신청 과목의 선수과목 검증
		if (!isClassAvailable(newClass, taken)) continue;
		ret = min(ret, minSemesterToGraduate(currSemester + 1, taken | newClass)+1);
	}

	// 휴학한 경우(남은 학기수를 추가하지 않음)
	ret = min(ret, minSemesterToGraduate(currSemester + 1, taken));

	return ret;
}

void readClassInfo(vector<int> &classInfoList, int total)
{
	for (int index = 0; index < total; index++)
	{
		int classCnt;
		fscanf(fpInput, "%d", &classCnt);

		for (int classIdx = 0; classIdx < classCnt; classIdx++)
		{
			int classInfo;
			fscanf(fpInput, "%d", &classInfo);
			classInfoList[index] |= (1 << classInfo);
		}
	}
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
		fscanf(fpInput, "%d %d %d %d\n", &totalClasses, &toCompleteClasses, &totalSemester, &maxClasses);

		// 전역변수 초기화
		memset(cache, -1, sizeof(cache));
		semesterClassInfo = vector<int>(totalSemester, 0);
		preClassInfo = vector<int>(totalClasses, 0);

		// 선수과목 정보
		readClassInfo(preClassInfo, totalClasses);
		// 학기정보
		readClassInfo(semesterClassInfo, totalSemester);

		int ret = minSemesterToGraduate(0, 0);
		if (ret == INF) 
		{
			fprintf(fpOutput, "IMPOSSIBLE\n");
			if (isFile) printf("IMPOSSIBLE\n");
		}
		else
		{
			fprintf(fpOutput, "%d\n", ret);
			if (isFile) printf("%d\n", ret);
		}
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