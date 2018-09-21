#ifndef _QUEUE_H__
#define _QUEUE_H__

#include <mutex>

class TQueue
{
private:
	int m_Head;
	int m_Tail;
	int m_Count;
	int m_Max;
	void **m_List;

public:
	TQueue(int Capacity);
	virtual ~TQueue();

	bool GetNode(void *&pNode);
	int PutNode(void *pNode);

	int GetCount();
	int GetCapacity();
	int GetHead();
	int GetTail();
	void Clear();
};


class StaticMemoryManager
{
private:
	void *m_Memory;
	TQueue *m_Queue;
	int m_Capacity;
	int m_BlockSize;

public:
	StaticMemoryManager(const int BlockSize, const int Capacity);
	virtual ~StaticMemoryManager();
	bool GetMem(void *&pMem);
	bool FreeMem(void *pMem);
	int Capacity();
	int FreeCount();

};

class ConcurrentMemoryManager : public StaticMemoryManager
{
public:
	ConcurrentMemoryManager(const int BlockSize, const int Capacity)
		: StaticMemoryManager(BlockSize, Capacity)
	{

	}

	~ConcurrentMemoryManager()
	{

	}

	inline bool GetMem(void *&pMem)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return StaticMemoryManager::GetMem(pMem);
	}

	inline bool FreeMem(void *pMem)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return StaticMemoryManager::FreeMem(pMem);
	}

	inline int Capacity()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return StaticMemoryManager::Capacity();
	}

	inline int FreeCount()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return StaticMemoryManager::FreeCount();
	}

private:
	std::mutex mMutex;
};



#endif //_QUEUE_H__
