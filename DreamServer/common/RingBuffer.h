/*
*This file is about RingBuffer
*/

#ifndef _RING_BUFFER_H__
#define _RING_BUFFER_H__

#include <functional>
#include <Base.h>

//#define RING_BUFFER_SIZE 1024

template <typename BufferType>
class RingBuffer
{
public:
	RingBuffer()
	{
		for (int i = 0; i < RING_BUFFER_SIZE; ++i)
		{
			rbBufferData[i] = BufferType();
		}
	}

	bool push(BufferType &data)
	{
		if (isFull()) return false;
		*rbBufferData[rbTail] = data;
		rbTail = (rbTail + 1) % RING_BUFFER_SIZE;
		return true;
	}

	bool pop(BufferType *data)
	{
		if (isEmpty()) return false;
		*data = rbBufferData[rbHead];
		rbHead = (rbHead + 1) % RING_BUFFER_SIZE;
		return true;
	}

	/*
	*enumerate buffer , example:
	*enumerate([&](BufferType* data)->bool
	*{
	*printf(data);
	*return true;
	*});
	*/
	void enumerate(std::function<bool(BufferType*)> func)
	{
		BufferType *data = nullptr;
		int size = 0;

		if (!popAll(data, &size))
		{
			SAFE_DELETE_ARRAY(data);
			return;
		}

		if (nullptr == data || 0 == size)
		{
			SAFE_DELETE_ARRAY(data);
			return;
		}

		for (int i = 0; i < size; ++i)
		{
			if (!func(&data[i]))
			{
				SAFE_DELETE_ARRAY(data);
				return;
			}
		}

		SAFE_DELETE_ARRAY(data);
		return;
	}
private:
	bool isEmpty()
	{
		return rbHead == rbTail;
	}

	bool isFull()
	{
		return rbHead == (rbTail + 1) % RING_BUFFER_SIZE;
	}

	bool popAll(BufferType *data, int *size)
	{
		if (isEmpty())
		{
			return false;
		}

		int count = rbHead < rbTail ? (rbTail - rbHead) : (RING_BUFFER_SIZE - rbHead + rbTail);
		data = new BufferType[count];
		*size = count;
		int tmpTail = rbTail;

		if (rbHead < tmpTail)
		{
			for (int i = rbHead; i < tmpTail; ++i)
			{
				data[i - rbHead] = rbBufferData[i];
			}
		}
		else
		{
			for (int i = rbHead; i < RING_BUFFER_SIZE; ++i)
			{
				data[i - rbHead] = rbBufferData[i];
			}

			for (int i = 0; i < rbTail; ++i)
			{
				data[RING_BUFFER_SIZE - rbHead + i] = rbBufferData[i];
			}
		}

		rbHead = tmpTail;

		return true;
	}
private:
	const static int RING_BUFFER_SIZE = 10;

	BufferType rbBufferData[RING_BUFFER_SIZE];
	int rbHead = 0;
	int rbTail = 0;
};


#endif //_RING_BUFFER_H__