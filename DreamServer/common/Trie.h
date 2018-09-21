///this file is about trie
///

#pragma once

#ifndef _TRIE_H__
#define _TRIE_H__

#include <iostream>
#include <cstring>
#include <vector>


/*****************************
*
*trieNode
*
*****************************/
template<int Size>
class TrieNode
{
public:
	typedef TrieNode* PTrieNode;
	TrieNode() 
	{
		for (int i = 0; i < Size;++i)
		{
			children[i] = nullptr;
		}
	}
	~TrieNode()
	{
		for (int i = 0; i < Size; ++i)
		{
			delete children[i];
			children[i] = nullptr;
		}
	}
public:
	int terminableSize = 0;
	int nodeSize = 0;
	PTrieNode children[Size];
};

/*****************************
*
*trie
*
*****************************/

template<int Size, class Type>
class Trie
{
public:
	typedef TrieNode<Size>	Node;
	typedef TrieNode<Size>*	PNode;
	Trie() {}

	template<class Iterator>
	void insert(Iterator beg, Iterator end);
	void insert(const char* str);

	template<class Iterator>
	bool find(Iterator beg, Iterator end);
	bool find(const char* str);

	template<class Iterator>
	bool downNodeAlone(Iterator beg);

	template<class Iterator>
	bool erase(Iterator beg, Iterator end);
	bool erase(const char* str);

	int totalNodes(PNode pNode);
	int totalNoReNodes(PNode pNode);

public:
	PNode root = new Node;
private:
	Type index;
};

template<int Size>
class IndexType
{
public:
	int operator[](char ch)
	{
		return ch % Size;
	}
};


template<int Size, class Type>
template<class Iterator>
void Trie<Size, Type>::insert(Iterator beg, Iterator end)
{
	PNode cur = root;
	PNode pre = nullptr;
	Iterator tmpBeg = beg, tmpEnd = end;
	while (tmpBeg++ != tmpEnd)
	{
		printf("1111111: %d\n",*tmpBeg);
		printf("2222222: %d\n",index[*tmpBeg]);
		if (!cur->children[index[*tmpBeg]])
		{
			cur->children[index[*tmpBeg]] = new Node;
			++cur->nodeSize;
		}

		pre = cur;
		cur = cur->children[index[*tmpBeg]];
	}

	++pre->terminableSize;
}

template<int Size, class Type>
void Trie<Size, Type>::insert(const char* str)
{
	insert(str, str + strlen(str));
}

template<int Size, class Type>
template<class Iterator>
bool Trie<Size, Type>::find(Iterator beg, Iterator end)
{
	PNode cur = root, pre = nullptr;
	Iterator tmpBeg = beg, tmpEnd = end;

	while (tmpBeg++ != tmpEnd)
	{
		if (nullptr == cur->children[index[*tmpBeg]])
		{
			return false;
		}
		else
		{
			pre = cur;
			cur = cur->children[index[*tmpBeg]];
		}
	}

	if (pre->terminableSize > 0)
	{
		return true;
	}

	return false;
}

template<int Size, class Type>
bool Trie<Size, Type>::find(const char* str)
{
	return find(str, str + strlen(str));
}

template<int Size, class Type>
template<class Iterator>
bool Trie<Size, Type>::downNodeAlone(Iterator beg)
{
	PNode cur = root;
	int terminalNum = 0;

	if (nullptr == cur->children[index[*beg]])
	{
		return false;
	}

	cur = cur->children[index[*beg]];

	while (cur->nodeSize != 0)
	{
		terminalNum += cur->terminableSize;

		if (cur->nodeSize > 1)
		{
			return false;
		}
		else
		{
			for (int i = 0; i < Size; ++i)
			{
				if (cur->children[i])
				{
					cur = cur->children[i];
				}
			}
		}
	}

	if (1 == terminalNum)
	{
		return true;
	}

	return false;
}

template<int Size, class Type>
template<class Iterator>
bool Trie<Size, Type>::erase(Iterator beg, Iterator end)
{
	if (!find(beg, end))
	{
		return false;
	}

	PNode cur = root, pre = nullptr;
	Iterator tmpBeg = beg;
	while (tmpBeg++ != end)
	{
		if (downNodeAlone(beg))
		{
			delete cur;
			return true;
		}

		pre = cur;
		cur = cur->children[index[*tmpBeg]];	
	}

	if (pre->terminableSize > 0)
	{
		--pre->terminableSize;
		return true;
	}

	return false;
}

template<int Size, class Type>
bool Trie<Size, Type>::erase(const char* str)
{
	if (find(str))
	{
		return erase(str, str + strlen(str));
	}

	return false;
}

template<int Size, class Type>
int Trie<Size, Type>::totalNodes(PNode pNode)
{
	if (nullptr == pNode)
	{
		return 0;
	}

	int size = pNode->terminableSize;

	for (int i = 0; i < Size; ++i)
	{
		size += totalNodes(pNode->children[i]);
	}

	return size;
}

template<int Size, class Type>
int Trie<Size, Type>::totalNoReNodes(PNode pNode)
{
	if (nullptr == pNode)
	{
		return 0;
	}

	int size = 0;
	if (pNode->terminableSize > 0)
	{
		size += 1;
	}

	for (int i = 0; i < Size; ++i)
	{
		size += totalNoReNodes(pNode->children[i]);
	}

	return size;
}




#endif //