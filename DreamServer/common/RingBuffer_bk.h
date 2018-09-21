//--------------------------------------
//This file is about ringbuffer 
//--------------------------------------

#ifndef _RING_BUFFER_H__
#define _RING_BUFFER_H__

#include <functional>

#define RING_BUFFER_SIZE 1024

#define SAFE_DELETE(s)	\
	delete [] s;	\
	s = nullptr;


template<typename BufferType>
class RingBuffer
{
public:
	 bool push(BufferType data)
	{
		if(isFull()) return false;

		mData[mTail] = data;
		mTail = (mTail + 1) % RING_BUFFER_SIZE;
		return true;
	}

	 BufferType pop()
	 {
		 if(isEmpty()) return (BufferType)0;

		 BufferType ret = mData[mHead];
		 mHead = (mHead + 1) % RING_BUFFER_SIZE;
		 return ret;
	 }

	 //enumerate data
	 //example:
	 /*
	  * enumerate([&](BufferType* data)->bool
	  * {
	  * printf(data);
	  * return true;
	  * })
	  */
	 void enumerate(std::function<bool (BufferType*)> func)
	 {
		 BufferType *data = nullptr;
		 int size = 0;

		 if(!popAll(data, &size)) return;

		 if(nullptr == data) return;

		 for(int i = 0; i < size; ++i)
		 {
			 if(!func(&data[i])) return;
		 }

		 delete [] data;
		 data = nullptr;
	 }

private:
	bool isFull() { return mHead == (mTail+1)%RING_BUFFER_SIZE; }

 	bool isEmpty() { return mHead == mTail; }

	bool popAll(BufferType *data, int *size)
	 {
		 if(isEmpty()) return false;

		 int tmpTail = mTail;
		 //int tmpHead = mHead;
		 int count = mHead < tmpTail ? (tmpTail-mHead):(RING_BUFFER_SIZE-mHead+tmpTail);
		 *size = count;
		 
		 data = new BufferType[count];
		 if(mHead < tmpTail)
		 {
			 for(int i = mHead; i < tmpTail; ++i)
			 {
				 data[i-mHead] = mData[i];
			 }
		 }
		 else
		 {
			 for(int i = mHead; i < RING_BUFFER_SIZE; ++i)
			 {
				 data[i-mHead] = mData[i];
			 }

			 for(int i = 0; i < tmpTail; ++i)
			 {
				 data[RING_BUFFER_SIZE-mHead+i] = mData[i];
			 }
		 }

		 mHead = tmpTail;

		 return true;
	 }
private:
	BufferType mData[RING_BUFFER_SIZE] = {0};
	int mHead = 0;
	int mTail = 0;
}

#endif //_RING_BUFFER_H__
