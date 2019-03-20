/**this file filled object's pool
*similiar as std::list<std::<vector>>
*
*/
#ifndef _OBJECT_POOL_H__
#define _OBJECT_POOL_H__

#include <vector>
#include <list>

#define CHUNKS_NUM	1
#define CHUNKS_RESERVE_NUM 128
#define NODES_NUM	128

template<class T>
class ChunkNodeT
{
public:
	ChunkNodeT(int capacity)
	{
		mTail = -1;
		mMax = capacity;
		mCount = mMax;
		mList = new T[mMax];
		//mList = (T*)malloc(sizeof(T) * mMax);
	}

	virtual ~ChunkNodeT()
	{
		for (int i = 0; i < mMax; ++i)
		{
			delete[] mList;
			mList = nullptr;
		}
	}

public:
	T* GetNode()
	{
		if (mCount <= 0) return nullptr;
		--mCount;

		return &mList[++mTail];
	}

	void PutNode(T *p) = delete;

	bool IsFull()
	{
		return mCount <= 0;
	}
	
private:
	int mTail;
	int mCount;
	int mMax;
	T *mList;
};

template <class T>
class ObjectPoolT
{
private:
	typedef ChunkNodeT<T>	ChunkNode;
	typedef ChunkNodeT<T>*	PChunkNode;
public:
	ObjectPoolT(int chunksNum = CHUNKS_NUM, int nodesNum = NODES_NUM)
		:mNodesNum(nodesNum),
		mChunksNum(chunksNum)
	{
		mChunksVec.reserve(CHUNKS_RESERVE_NUM);
		for (int i = 0; i < mChunksNum; ++i)
		{
			mChunksVec.push_back(new ChunkNode(mNodesNum));
		}
	}

	virtual ~ObjectPoolT()
	{
		for (auto pn : mChunksVec)
		{
			delete pn;
			pn = nullptr;
		}
	}

	T* GetFreeObject()
	{
		T *p = nullptr;
		if (!mFreeNodeList.empty())
		{
			p =  mFreeNodeList.front();
			mFreeNodeList.pop_front();
		}
		else
		{
			for (auto pn : mChunksVec)
			{
				if (!pn->IsFull())
				{
					p = pn->GetNode();
					break;
				}
			}
		}

		//have no free node
		if (nullptr == p)
		{
			p = ExpansionPool();
		}
		
		return new(p)T();

	}

	void RecycleObject(T *p)
	{
		p->~T();
		mFreeNodeList.push_back(p);
	}

private:
	T* ExpansionPool()
	{
		PChunkNode pNewChunk = new ChunkNode(mNodesNum);
		mChunksVec.push_back(pNewChunk);
		return pNewChunk->GetNode();
	}
	
private:
	int mNodesNum;
	int mChunksNum = 1;
	std::list<T*> mFreeNodeList;
	std::vector<PChunkNode> mChunksVec;
};





#endif //_OBJECT_POOL_H__