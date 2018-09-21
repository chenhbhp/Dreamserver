#ifndef _MSG_PROCESSOR_H__
#define _MSG_PROCESSOR_H__

#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>

class MsgProcessor
{
public:
	typedef std::function<void(void)>	FUNC_TYPE;

public:
	MsgProcessor()
	{
	}

	virtual ~MsgProcessor()
	{
	}

	MsgProcessor(const MsgProcessor&) = delete;
	MsgProcessor& operator=(const MsgProcessor&) = delete;

public:
	bool start(int threadNum)
	{
		if (mIsStart)
		{
			return false;
		}

		for (int i = 0; i < threadNum; ++i)
		{
			mThreads.push_back(std::unique_ptr<std::thread>(
				new std::thread(
				std::bind(&MsgProcessor::run, this))
				)
				);
		}

		mIsStart = true;
		return true;
	}

	bool processOnce()
	{
		bool ret = false;
		try
		{
			FUNC_TYPE func;

			mMutex.lock();
			if (!mMsgQueue.empty())
			{
				func = mMsgQueue.front();
				mMsgQueue.pop_front();
				mMutex.unlock();
			}
			else
			{
				mMutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			if (nullptr != func)
			{
				func();
				ret = true;
			}
		}
		catch (std::exception &e)
		{
			printf("MsgProcessor exception: %s", e.what());
		}
		catch (...)
		{
		}

		return ret;
	}

	bool post(FUNC_TYPE func)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (!mIsStart)
		{
			return false;
		}

		mMsgQueue.push_front(func);

		return true;
	}

	void run()
	{
		while (true)
		{
			processOnce();
			if (!mIsStart && mMsgQueue.empty())
			{
				break;
			}
		}
	}

private:
	std::mutex	mMutex;
	bool mIsStart = false;
	std::deque<FUNC_TYPE> mMsgQueue;
	std::vector<std::unique_ptr<std::thread>> mThreads;
};



#endif //_MSG_PROCESSOR_H__