
#ifndef _ITEM_H__
#define _ITEM_H__

#include <Base.h>

enum ItemFromE
{
	ITEM_FROM_NONE = 0,
	ITEM_FROM_DB,
	ITEM_FROM_DAILY,
};

class Item :public Object
{
public:
	Item();
	virtual ~Item();
public:
	DST::uint32_t getItemID() { return mItemID; }
	DST::uint32_t getItemUUID() { return mItemUUID; }
	ItemFromE getItemFrom() { return mItemFrom; }
	void setItemID(DST::uint32_t itemid) { mItemID = itemid; }
	void setItemUUID(DST::uint32_t itemuuid) { mItemUUID = itemuuid; }
	void setItemFrom(ItemFromE itemfrom) { mItemFrom = itemfrom; }

private:
	DST::uint32_t mItemID = 0;
	DST::uint32_t mItemUUID = 0;
	ItemFromE mItemFrom = ITEM_FROM_NONE;
};


#endif //_ITEM_H__