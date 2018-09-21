#include "Queue.h"
#include <stdlib.h>

TQueue::TQueue(int Capacity)
{
	m_Max = Capacity;
	m_Count = 0;
	m_Head = m_Tail = -1;
	m_List = new void*[Capacity];
	Clear();
}

TQueue::~TQueue()
{
	delete[] m_List;
	m_List = nullptr;
}

void TQueue::Clear()
{
	m_Count = 0;
	m_Head = m_Tail = -1;
	for (int i = 0; i < m_Max; ++i)
	{
		m_List[i] = nullptr;
	}
}

int TQueue::PutNode(void *pNode)
{
	if (m_Count >= m_Max)
	{
		return -1;
	}
	else
	{
		int Idx = (++m_Tail) % m_Max;
		m_List[Idx] = pNode;
		++m_Count;
		return Idx;
	}
}

bool TQueue::GetNode(void *&pNode)
{
	pNode = nullptr;
	if (m_Count <= 0) return false;
	--m_Count;
	pNode = m_List[(++m_Head) % m_Max];
	return true;
}

int TQueue::GetCapacity()
{
	return m_Max;
}

int TQueue::GetCount()
{
	return m_Count;
}

int TQueue::GetHead()
{
	return m_Head;
}

int TQueue::GetTail()
{
	return m_Tail;
}


///////////////////StaticMemoryManager//////////////
StaticMemoryManager::StaticMemoryManager(const int BlockSize, const int Capacity)
{
	m_BlockSize = BlockSize;
	m_Capacity = Capacity;
	m_Memory = malloc(Capacity * BlockSize);
	if (nullptr == m_Memory) throw("GlobalAlloc Error!");

	m_Queue = new TQueue(m_Capacity);
	for (int i = 0; i < m_Capacity; ++i)
	{
		m_Queue->PutNode((void*)((long long)m_Memory + BlockSize*i));
	}
}

StaticMemoryManager::~StaticMemoryManager()
{
	delete m_Queue;
	m_Queue = nullptr;
	free(m_Memory);
	m_Memory = nullptr;
}

bool StaticMemoryManager::GetMem(void *&pMem)
{
	return m_Queue->GetNode(pMem);
}

bool StaticMemoryManager::FreeMem(void *pMem)
{
	return m_Queue->PutNode(pMem) > -1;
}


int StaticMemoryManager::Capacity()
{
	return m_Capacity;
}

int StaticMemoryManager::FreeCount()
{
	return m_Capacity - m_Queue->GetCount();
}


