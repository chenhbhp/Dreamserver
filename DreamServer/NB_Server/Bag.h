
#ifndef _BAG_H__
#define _BAG_H__

#include <Base.h>
#include "Item.h"

///store in heap
template <class ItemType = Item>
class BagBase :public Object
{
public:
	static const DST::uint8_t MAX_SLOTSIZE = 255;

	BagBase() {}
	virtual ~BagBase()
	{
		if (mItemCount != 0)
		{
			for (DST::uint8_t i = 0; i < MAX_SLOTSIZE; ++i)
			{
				if (nullptr != mBagSlots[i])
				{
					SAFE_DELETE(mBagSlots[i]);
				}
			}
		}
	}
public:
	bool isFull() { return mItemCount == MAX_SLOTSIZE; }

	ItemType* addItem(DST::uint32_t itemid, ItemFromE itemfrom)
	{
		//todo: chect itemid

		ItemType *pRetItem = new ItemType;
		pRetItem->setItemID(itemid);
		//pRetItem->setItemUUID();
		mBagSlots[mItemCount++] = pRetItem;

		return pRetItem;
	}

	bool delItem(ItemType* pItem)
	{
		return true;
	}

	void destroyItem(ItemType* pItem)
	{
		SAFE_DELETE(pItem);
	}

private:
	ItemType* mBagSlots[MAX_SLOTSIZE] = { nullptr };
	DST::uint8_t mItemCount = 0;
};

///store in stack
template <class ItemType>
class Bag : public Object
{
public:
	Bag() {}
	virtual ~Bag();
public:
	ItemType* addItem(DST::uint32_t itemid, ItemFromE itemfrom)
	{
		ItemType *pRetItem = nullptr;

		ItemType tempItem;
		DST::uint32_t uuid = 0;
		//todo: generate uuid

		auto insertRet = mItemMap.insert(std::make_pair(uuid, tempItem));
		if (!insertRet.second)
		{
			return nullptr;
		}

		pRetItem = &(insertRet.first->second);
		return pRetItem;
	}

	bool delItem(DST::uint32_t itemUUID)
	{
		auto it = mItemMap.find(itemUUID);
		if (it == mItemMap.end())
		{
			return false;
		}

		mItemMap.erase(it);
		return true;
	}
private:
	typedef std::map<DST::uint32_t,ItemType> ItemTypeMap;
	ItemTypeMap mItemMap;
};


#endif // _BAG_H__