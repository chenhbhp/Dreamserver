#pragma once

#ifndef _UTILS_H__
#define _UTILS_H__

#include <ctime>
#include <stdint.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace Utils
{
#ifdef WIN32
	//SYSTEMTIME sysTime;
	//GetLocalTime(&sysTime);
#else
	//gettimeofday();
#endif

	//------------------------------------------------------------------
	//time
	//------------------------------------------------------------------
	time_t start_of_day()
	{
		time_t now_time = time(NULL);
		struct tm* data = localtime(&now_time);
		data->tm_hour = 0;
		data->tm_min = 0;
		data->tm_sec = 0;
		return mktime(data);
	}

	time_t end_of_day()
	{
		return start_of_day() + 24 * 3600;
	}

	time_t noon_of_day()
	{
		return start_of_day() + 12 * 3600;
	}

	time_t end_of_month()
	{
		time_t now_time = time(NULL);
		struct tm* data = localtime(&now_time);

		long long max_day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		long long month_max_day = max_day[data->tm_mon];
		if (data->tm_year % 4 == 0 && data->tm_mon == 1)
		{
			++month_max_day;
		}

		return end_of_day() + (month_max_day - data->tm_mday) * 24 * 3600;
	}


	//------------------------------------------------------------------
	//number
	//------------------------------------------------------------------
	unsigned int sum_of_arithmetic_progression(unsigned int low_bound, unsigned int high_bound, unsigned int diffValue = 1)
	{
		unsigned int firstVal = low_bound;
		if (0 == low_bound)
		{
			++firstVal;
		}
		unsigned int size = high_bound - firstVal + 1;
		
		return size*firstVal + ((size*(size - 1)*diffValue) >> 1);
		//return size*firstVal + size*(size - 1)*diffValue / 2;
	}

	unsigned int middle_of_two_numbers(unsigned int low, unsigned int upper)
	{
		return low + ((upper - low) >> 1);
	}


	//------------------------------------------------------------------
	//sscanf() and regular expression
	//------------------------------------------------------------------
	void sscanf_between_two_split_strs()
	{
		char buf[20];
		const char* splitStr = "%*[^_]_%[^.]";
		sscanf("Headportrait_123.png", splitStr, buf);

		printf("%s\n", buf);	// print: 123
	}

	//------------------------------------------------------------------
	//algorithm of sort
	//------------------------------------------------------------------
	template<typename T>
	void swap(T *t1, T *t2)
	{
		T temp = *t1;
		*t1 = *t2;
		*t2 = temp;
	}

	///冒泡排序，稳定
	void bubbleSort(int a[], int size)		//time: O(n^2)
	{
		for (int i = 0; i < size;++i)
		{
			for (int j = i + 1; j < size; ++j)
			{
				if (a[i] < a[j])
				{
					int tmp = a[i];
					a[i] = a[j];
					a[j] = tmp;
				}
			}
		}
	}

	///鸡尾酒排序(双向冒泡排序)，稳定
	void cockTailSort(int a[], int len)		//time: O(n^2)
	{
		bool isSwapped = true;
		int bound = 0, beg_p = 0, end_p = len - 1;

		while (isSwapped)
		{
			isSwapped = false;
			for (int i = 0; i < end_p;++i)
			{
				if (a[i] > a[i+1])
				{
					swap<int>(&a[i], &a[i+1]);
					bound = i;
					isSwapped = true;
				}
			}
			end_p = bound;

			for (int i = end_p; i > beg_p;--i)
			{
				if (a[i] < a[i-1])
				{
					swap<int>(&a[i], &a[i-1]);
					bound = i;
					isSwapped = true;
				}
			}
			beg_p = bound;
		}
	}

	///快速排序，不稳定，对于大范围乱序是最快排序方法
	void quickSort(int a[], int begP, int endP)		//time: O(n*logn)
	{
		if (begP >= endP)
		{
			return;
		}

		int i = begP, j = endP;
		int tmp = a[i];
		while (i < j)
		{
			while ((i < j) && (a[j] > tmp)) --j;
			if (i < j)
			{
				a[i] = a[j];
			}

			while ((i < j) && (a[i] < tmp)) ++i;
			if (i < j)
			{
				a[j] = a[i];
			}
		}

		a[i] = tmp;
		quickSort(a, begP, i - 1);
		quickSort(a, i + 1, endP);
	}

	///鸽巢排序,
	//在不可避免遍历每一个元素并且排序的情况下效率最好的一种排序算法.
	//但它只有在差值(或者可被映射在差值)很小的范围内的数值排序的情况下实用
	//
	//一是数组中存储的必须是int类型或者转换为int类型不丢失真实数据的数据类型；
	//二是必须事先预测到数组中存储的最大元素，在程序运行过程中求得后因为不是const值，所以无法用int b[Max] = {0};来初始化数组b.
	const int MAX_NUM = 10;
	void pigeonHoleSort(int a[], int size, const int max_val)
	{
		int temp[MAX_NUM] = { 0 };
		for (int i = 0; i < size;++i)
		{
			++temp[a[i]];
		}

		int j = 0;
		for (int i = 0; i < max_val; ++i)
			for (int k = 0; k < temp[i];++k)
			{
				a[j++] = i;
			}
	}
	void radixSort()		//基数排序, time: O(n)
	{

	}

	//堆排序：不稳定排序
	//http://blog.csdn.net/puqutogether/article/details/43195703
	// 1.初始化堆，调整堆
	// 2.堆排序
	///调整堆函数
	void heapAdjust(int a[], int idx, int size)
	{
		if (idx > size/2)
		{
			return;
		}

		int lchild = 2 * idx + 1;
		int rchild = 2 * idx + 2;
		int temp = idx;

		//max heap
		if (lchild <= size-1 && a[lchild] > a[temp])
		{
			temp = lchild;
		}

		if (rchild <= size-1 && a[rchild] > a[temp])
		{
			temp = rchild;
		}

		if (idx != temp)
		{
			swap<int>(&a[idx], &a[temp]);
			heapAdjust(a, temp, size);
		}

	}

	//从非叶子节点最大号值size/2开始遍历,
	void buildHeap(int a[], int size)
	{
		for (int i = size / 2 - 1; i >= 0;--i)
		{
			heapAdjust(a, i, size);
		}
	}

	void heapSort(int a[], int size)
	{
		buildHeap(a, size);
		for (int i = size - 1; i > 0;--i)
		{
			swap<int>(&a[0], &a[i]);
			heapAdjust(a, 0, i);
		}
	}


	////////////////charset/////////////////////////
	bool Unicode2UTF(std::vector<char> &Dest, wchar_t *szSrc)
	{
		int iTextLen = wcslen(szSrc);
		if (iTextLen == 0)
		{
			return false;
		}

		iTextLen = WideCharToMultiByte(CP_UTF8,
			0,
			(LPWSTR)szSrc,
			-1,
			NULL,
			0,
			NULL,
			NULL);
		if (iTextLen == 0)
			return false;
		Dest.resize(iTextLen);
		::WideCharToMultiByte(CP_UTF8,
			0,
			(LPWSTR)szSrc,
			-1,
			&*Dest.begin(),
			iTextLen,
			NULL,
			NULL);
		return true;

	}

	//example:
	/*_variant_t variant;
	if (pDataBase->GetFieldValue("myname", variant))
	{
	if (variant.vt != VT_EMPTY && variant.vt != VT_NULL)
	{
	vector<char> vcName;
	if (chatbase::SomeUtils::Unicode2UTF(vcName, (wchar_t*)variant.pcVal))
	{
	if (vcName.size() > 0)
	{
	memcpy(usr->basicInfo.nickname, &*vcName.begin(), vcName.size() > 63 ? 63 : vcName.size());
	}
	}
	}
	}*/

	bool MBToUTF8(std::vector<char>& pu8, const char* pmb, uint32_t len)
	{
		// convert an MBCS string to widechar   
		uint32_t nlen = MultiByteToWideChar(CP_ACP, 0, pmb, -1, NULL, 0);

		WCHAR* lpszW = NULL;
		assert(nlen >= 0);
		lpszW = new WCHAR[nlen];


		uint32_t nrt = MultiByteToWideChar(CP_ACP, 0, pmb, -1, lpszW, nlen);

		if (nrt != nlen)
		{
			delete[] lpszW;
			return false;
		}
		// convert an widechar string to utf8  
		uint32_t utf8_len = WideCharToMultiByte(CP_UTF8, 0, lpszW, -1, NULL, 0, NULL, NULL);
		if (utf8_len <= 0)
		{
			delete[] lpszW;
			return false;
		}
		pu8.resize(utf8_len);
		nrt = WideCharToMultiByte(CP_UTF8, 0, lpszW, -1, &*pu8.begin(), utf8_len, NULL, NULL);
		delete[] lpszW;

		if (nrt != utf8_len)
		{
			pu8.clear();
			return false;
		}
		return true;
	}

	bool GB2312ToUTF8(const char *input, std::string& out, const char* type)
	{
		std::vector<char> tmp_vec;
		bool r = MBToUTF8(tmp_vec, input, strlen(input) + 1);
		if (r)
			out.assign(tmp_vec.begin(), tmp_vec.end());
		else
			out.assign(type);
		return r;
	}

	bool UTF8ToMB(std::vector<char>& pmb, const char* pu8, uint32_t utf8_len)
	{
		// convert an UTF8 string to widechar   
		unsigned long len = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8_len, NULL, 0);

		WCHAR* lpszW = NULL;
		lpszW = new WCHAR[len];


		uint32_t nrt = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8_len, lpszW, len);

		if (nrt != len)
		{
			delete[] lpszW;
			return false;
		}

		// convert an widechar string to Multibyte   
		unsigned long mb_len = WideCharToMultiByte(CP_ACP, 0, lpszW, len, NULL, 0, NULL, NULL);
		if (mb_len <= 0)
		{
			delete[] lpszW;
			return false;
		}
		pmb.resize(mb_len);
		nrt = WideCharToMultiByte(CP_ACP, 0, lpszW, len, &*pmb.begin(), mb_len, NULL, NULL);
		delete[] lpszW;

		if (nrt != mb_len)
		{
			pmb.clear();
			return false;
		}
		return true;
	}

}// namespace Utils




#endif