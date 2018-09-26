#ifndef _QUEUE_LOCK_FREE_H__
#define _QUEUE_LOCK_FREE_H__


template <typename T>
typedef struct _Node
{
	T data;
	_Node* prev;
	_Node* next;

	_Node()
	{
		prev = next = nullptr;
	}

	_Node(T data_)
		:data(data_)
	{
		prev = next = nullptr;
	}

}QNode, *PNode;




#endif  //_QUEUE_LOCK_FREE_H__