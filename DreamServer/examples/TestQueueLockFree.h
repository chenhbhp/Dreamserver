
#ifndef _TEST_QUEUE_LOCK_FREE_H__
#define _TEST_QUEUE_LOCK_FREE_H__

#include "../base/QueueLockFree.h"
#include <thread>
#include <chrono>
#include <vector>
#include <functional>

#define TH_NUM 10

typedef std::function<void (int)>	 FUNC_TYPE;

static TQueueLockFree<int> tq;

void ThreadFunc(int val_)
{
	while (true)
	{
		tq.push(val_);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::this_thread::yield();
	}
}

void TestQueueLockFree()
{
	std::vector<std::thread> thVec;
	for (int i = 0; i < TH_NUM; ++i) thVec.push_back(std::thread(ThreadFunc, i));
	//wrong operation: std::bind(ThreadFunc, nullptr, i), bind绑定成员函数

	int val = 0;
	const int linelimit = 10;
	while (true)
	{
		int sz = tq.size();
		for (int i = 0; i < sz; ++i)
		{
			tq.pop(val);
			std::cout << val << ", ";
			if (i != 0 && (i%linelimit == 0)) std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::this_thread::yield();
	}

	//finish threads before main thread stop
	for (auto &th : thVec) th.join();
}



#endif  //_TEST_QUEUE_LOCK_FREE_H__