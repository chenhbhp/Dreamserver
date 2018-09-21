/************************************************************************/
/* The file is about implement of hash table
*/
/************************************************************************/

#ifndef _HASH_TABLE_H__
#define _HASH_TABLE_H__

#include<functional>
#include<Base.h>

#define HASH_TABLE_LEN 256

template<typename T>
struct _Link_Node
{
	DST::uint32_t id;
	T data;
	struct _Link_Node<T> *next;
};

template<typename T>
struct _Hash_Header
{
	struct _Link_Node<T> *next;
};

template<typename T>
class HashTable
{
public:
	typedef _Link_Node<T>		Link_Node;
	typedef _Link_Node<T>*		Link_Node_Ptr;
	typedef _Hash_Header<T>		Hash_Header;
	typedef _Hash_Header<T>*	Hash_Header_Ptr;
public:
	HashTable()
	{
		for (DST::uint8_t i = 0; i < HASH_TABLE_LEN; ++i)
		{
			hashTable[i]->next = nullptr;
		}
	}

	virtual ~HashTable()
	{
		for (DST::uint8_t i = 0; i < HASH_TABLE_LEN;++i)
		{
			if (nullptr != hashTable[i]->next)
			{
				Hash_Header_Ptr pHeader = hashTable[i];
				Link_Node_Ptr node = pHeader->next;
				do 
				{
					pHeader->next = node->next;
					SAFE_DELETE(node);
					node = pHeader->next;
				} while (nullptr != node);
			}
		}
	}

	void appendNode(DST::uint32_t id, T data)
	{
		Link_Node_Ptr node = new Link_Node;
		node->id = id;
		node->data = std::move(data);
		node->next = nullptr;

		insertLinkNode(node);
	}

	Link_Node_Ptr findNode(DST::uint32_t id)
	{
		return findLinkNode(id);
	}

	void delNode(DST::uint32_t id)
	{
		eraseLinkNode(id);
	}

	void delNode(Link_Node_Ptr node)
	{
		eraseLinkNode(node);
	}

	Link_Node_Ptr operator[](DST::uint32_t id)
	{
		return findLinkNode(id);
	}

	void enumerate(std::function<bool(Link_Node_Ptr)> func)
	{
		for (DST::uint8_t i = 0; i < HASH_TABLE_LEN;++i)
		{
			if (nullptr != hashTable[i]->next)
			{
				Link_Node_Ptr node = hashTable[i]->next;
				while (nullptr != node->next)
				{
					if (!func(node)) return;
				}
			}
		}
	}

private:
	Link_Node_Ptr initLinkNode()
	{
		Link_Node_Ptr pNode = new Link_Node;
		pNode->next = nullptr;
		return pNode;
	}

	Hash_Header_Ptr initHashHeaderNode()
	{
		Hash_Header_Ptr pHeader = new Hash_Header;
		pHeader->next = nullptr;
		return pHeader;
	}

	DST::uint8_t hashFuncMod(DST::uint32_t id)
	{
		return id % HASH_TABLE_LEN;
	}

	void insertLinkNode(Link_Node_Ptr new_node)
	{
		new_node->next = nullptr;
		DST::uint8_t pos = hashFuncMod(new_node->id);
		if (nullptr == hashTable[pos]->next)
		{
			hashTable[pos]->next = new_node;
		}
		else
		{
			Link_Node_Ptr node = hashTable[pos]->next;
			while (nullptr != node->next) node = node->next;

			node->next = new_node;
		}
	}

	Link_Node_Ptr findLinkNode(Link_Node_Ptr pNode)
	{
		return findLinkNode(pNode->id);
	}

	void eraseLinkNode(Link_Node_Ptr old_node)
	{
		eraseLinkNode(old_node->id);
	}

	Link_Node_Ptr findLinkNode(DST::uint32_t id)
	{
		DST::uint8_t pos = hashFuncMod(id);
		Link_Node_Ptr node = hashTable[pos]->next;

		while (nullptr != node && id != node->id)
			node = node->next;

		return node;
	}

	void eraseLinkNode(DST::uint32_t id)
	{
		Link_Node_Ptr pNode = findLinkNode(id);
		if (nullptr == pNode) return;

		DST::uint8_t pos = hashFuncMod(pNode->id);
		Hash_Header_Ptr pHashHeader = hashTable[pos];
		Link_Node_Ptr node = pHashHeader->next;
		if (pNode == node)
		{
			pHashHeader->next = pNode->next;
			SAFE_DELETE(pNode);
			return;
		}
		
		while (pNode != node->next)
			node = node->next;

		node->next = pNode->next;
		SAFE_DELETE(pNode);
		return;
	}
	
private:
	Hash_Header_Ptr hashTable[HASH_TABLE_LEN];
};


//////////////////implement by C//////////////////////
//typedef struct _Link_Node
//{
//	DST::uint32_t id;
//	T data;
//	struct _Link_Node *next;
//}Link_Node, *Link_Node_Ptr;
//
//typedef struct _Hash_Header
//{
//	struct _Link_Node *next;
//}Hash_Header, *Hash_Header_Ptr;
//
//Hash_Header_Ptr Hash_Table[HASH_TABLE_LEN];
//
////////////////////////////////////////////////
//DST::uint8_t Hash_Func_Mod(DST::uint32_t id)
//{
//	DST::uint8_t pos;
//	pos = id % HASH_TABLE_LEN;
//	return pos;
//}
//
////////////////////////////////////////////////
//Link_Node_Ptr init_link_node()
//{
//	Link_Node_Ptr node;
//	node = (Link_Node_Ptr)malloc(sizeof(Link_Node));
//	node->next = NULL;
//
//	return node;
//}
//
//Hash_Header_Ptr init_hash_header_node()
//{
//	Hash_Header_Ptr header;
//	header = (Hash_Header_Ptr)malloc(sizeof(Hash_Header));
//	header->next = NULL;
//
//	return header;
//}
//
/////////////////////////////////////////////////////////////
//void init_hash_table()
//{
//	for (DST::uint8_t i = 0; i < HASH_TABLE_LEN;++i)
//	{
//		Hash_Table[i] = init_hash_header_node();
//	}
//}
//
//void append_link_node(Link_Node_Ptr new_node)
//{
//	DST::uint8_t pos = Hash_Func_Mod(new_node->id);
//}



#endif //_HASH_TABLE_H__