/*测试结果：
*10万： 平均时间18微秒左右
*100万：平均时间187微秒左右
*
*/

#ifndef _TEST_OBJECT_POOL_H__
#define _TEST_OBJECT_POOL_H__

#include <fstream>
#include "../base/ObjectPool.h"

typedef struct _Temp
{
	int index = 3333;
	~_Temp()
	{
		index = 0;
	}
}TempNode, *PTempNode;

const int NUM = 1000000;
void TestObjectPool()
{
	std::ofstream ouFile("temp.txt", std::ios::out);
	ObjectPoolT<TempNode> temp;
	PTempNode pt = temp.GetFreeObject();
	temp.RecycleObject(pt);

	

	for (int i = 0; i < NUM; ++i)
	{
		LARGE_INTEGER nFreq, nBegTime, nEndTime;
		double test_time;
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBegTime);

		pt = temp.GetFreeObject();

		QueryPerformanceCounter(&nEndTime);
		test_time = (double)(nEndTime.QuadPart - nBegTime.QuadPart) / (double)nFreq.QuadPart;
		char tempChar[128];
		sprintf(tempChar, "%lf \n", test_time * 1000000); //(微秒)
		ouFile << tempChar;
		int j = 0;
	}

	ouFile.close();
}


#endif //_TEST_OBJECT_POOL_H__