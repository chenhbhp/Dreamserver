
#pragma once

#ifndef _DATA_STRUCT_H__
#define _DATA_STRUCT_H__

#pragma warning(disable:4996 4244)

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace DST		//DataStruct
{
	typedef unsigned char		uint8_t;
	typedef unsigned short		uint16_t;
	typedef unsigned int		uint32_t;
	typedef unsigned long long	uint64_t;

	typedef signed char			int8_t;
	typedef signed short		int16_t;
	typedef signed int			int32_t;
	typedef signed long long	int64_t;

	typedef std::vector<std::string> CStrVec;
	typedef std::vector<std::vector<std::string>> CStrVecVec;


	class AnyData
	{
	public:
		template<typename RealDataType>
		AnyData(RealDataType* data)
			:mRealData(data)
		{
		}

		template<typename RealDataType>
		RealDataType& getRealData()
		{
			return *reinterpret_cast<RealDataType*>(mRealData);
		}

		template<typename RealDataType>
		const RealDataType& getRealData() const
		{
			return *reinterpret_cast<const RealDataType*>(mRealData);
		}

		template<typename RealDataType>
		AnyData& operator=(RealDataType* data)
		{
			set<RealDataType>(data);
			return *this;
		}

	private:
		template<typename RealDataType>
		void set(RealDataType* data)
		{
			mRealData = reinterpret_cast<void*>(data);
		}
		
	private:
		void* mRealData;
	}; //class AnyData

	////////BSTree//////////////
#define ED_LESS_THAN(ed1, ed2)		\
(									\
(ed1.left < ed2.left) || ((ed1.left == ed2.left) && (ed1.right < ed2.right))	\
)
	


	template<typename ExtraType>
	struct ExtraData 
	{
		ExtraType left;
		ExtraType right;

		ExtraData()
		{
			left = ExtraType(0);
			right = ExtraType(0);
		}

		bool operator==(ExtraData& extra)
		{
			return this->left == extra.left && this->right == extra.right;
		}
	};

	template<typename ExtraType>
	class ExtraArray
	{
	public:
		uint32_t size()
		{
			return mExtraVec.size();
		}

		void insert(ExtraData<ExtraType> data)
		{
			mExtraVec.push_back(data);
		}

		void erase(ExtraData<ExtraType>& data)
		{
			mExtraVec.erase(mExtraVec.begin()+find(data));
		}

		uint32_t find(ExtraData<ExtraType>& data)
		{
			for (uint32_t i = 0; i < size();++i)
			{
				if (mExtraVec[i] == data)
				{
					return i;
				}
			}

			return -1;
		}

		const std::vector<ExtraData<ExtraType>>& getDataList() const
		{
			return mExtraVec;
		}

		void sort()
		{
			//bubbleSort();
			quickSort(0, mExtraVec.size() - 1);
			//binarySort();
		}
	private:
		void bubbleSort()		//time: O(n^2)
		{
			if (mExtraVec.empty())
			{
				return;
			}

			ExtraData<ExtraType> tmpExData;
			bool isNeedExchange = false;
			for (size_t i = 0; i < mExtraVec.size();++i)
			{
				for (size_t j = i + 1; j < mExtraVec.size();++j)
				{
					isNeedExchange = (mExtraVec[i].left < mExtraVec[j].left) 
						|| (mExtraVec[i].left == mExtraVec[j].left && mExtraVec[i].right < mExtraVec[j].right);

					if (isNeedExchange)
					{
						tmpExData		= mExtraVec[i];
						mExtraVec[i]	= mExtraVec[j];
						mExtraVec[j]	= tmpExData;
					}
				}
			}
		}
		
		void quickSort(int begP, int endP)		//time: n*logn
		{
			/*if (mExtraVec.empty())
			{
			return;
			}*/

			if (begP >= endP)
			{
				return;
			}

			ExtraData<ExtraType> tmpExData = mExtraVec[begP];
			int i = begP, j = endP;
			while (i < j)
			{
				while (i < j && ED_LESS_THAN(mExtraVec[j], tmpExData)) --j;
				if (i < j)
				{
					mExtraVec[i] = mExtraVec[j];
				}

				while (i < j && ED_LESS_THAN(tmpExData, mExtraVec[i])) ++i;
				if (i < j)
				{
					mExtraVec[j] = mExtraVec[i];
				}
				
			}

			mExtraVec[i] = tmpExData;
			quickSort(begP, i-1);
			quickSort(i+1, endP);
		}

		void binarySort()
		{

		}

	private:
		std::vector<ExtraData<ExtraType>> mExtraVec;
	};// struct ExtraArray
}



#endif