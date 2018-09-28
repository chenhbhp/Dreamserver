
#ifndef _QUEUE_LOCK_FREE_H__
#define _QUEUE_LOCK_FREE_H__

#include <atomic>

#include <Base.h>


template <typename T>
struct _Node
{
	T data;
	_Node* next;

	_Node()
	{
		data = T(-1);
		next = nullptr;
	}

	_Node(T data_)
		:data(data_)
	{
		next = nullptr;
	}
};

template <typename T>
class TQueueLockFree
{
public:
	typedef struct _Node<T> QNode, *PNode;

public:
	TQueueLockFree()
	{
		PNode node = new QNode();
		mHead = node;
	}

	virtual ~TQueueLockFree()
	{
		PNode node = mHead;
		while (node->next)
		{
			PNode n = node->next;
			SAFE_DELETE(node);
			node = n;
		}

		SAFE_DELETE(node);
	}

public:
	void push(const T& data_) throw()
	{
		PNode pnode = new QNode(data_);
		push(pnode);
	}

	void push(PNode pnode_) throw()
	{
		pnode_->next = mHead.load();
		while (!mHead.compare_exchange_weak(pnode_->next, pnode_));

		++mSize;
	}
	bool pop(T &data_)
	{
		if (0 == mSize || mHead.load()->next == nullptr) return false;

		PNode oldNode = mHead.load();
		while (!mHead.compare_exchange_strong(oldNode, oldNode->next));

		data_ = oldNode->data;
		oldNode->next = nullptr;
		SAFE_DELETE(oldNode);

		--mSize;

		return true;
	}

	size_t size() { return mSize;  }
private:
	std::atomic<PNode> mHead;
	std::atomic<PNode> mTail;
	size_t mSize = 0;
};






#endif  // _QUEUE_LOCK_FREE_H__