/// trie by map or hash_map

#pragma once

#ifndef _MAP_TRIE_H__
#define _MAP_TRIE_H__


#if WIN32
#define CH_SIZE 3	// 2 for windows, 3 for Linux

#elif linux
#define CH_SIZE 3

#endif

#include <windows.h>
#include <cstring>
#include <cctype>
#include <ctime>
#include <map>
#include <unordered_map>


struct MapTrieNode
{
	typedef std::map<std::string, MapTrieNode*> TrieMap;

	int terminalSize = 0;	//count that ends by this char
	TrieMap children;		//map or hash_map 
};

struct HashMapTrieNode
{
	typedef std::unordered_map<std::string, HashMapTrieNode*> TrieHashMap;

	int terminalSize = 0;
	TrieHashMap children;
};

template<size_t Size>
class CharAlter
{
public:
	CharAlter()
	{
		memset(ch, 0, CH_SIZE);
	}

	void GetChar(const char* str)
	{
		memcpy(ch, str, Size);
	}

	const char* C_Str()
	{
		return ch;
	}

private:
	char ch[Size];
};

template<class NodeType>
class MapTrie
{
public:
	typedef NodeType	Node;
	typedef NodeType*	PNode;

public:
	MapTrie() {}
	virtual ~MapTrie() {}
	const PNode getRoot() { return root; }

public:
	bool insert(const char* str)
	{
		if (nullptr == root || nullptr == str)
		{
			return false;
		}

		PNode cur = root,pre = nullptr;

		size_t len = strlen(str);
		if (0 == len)
		{
			return false;
		}

		for (size_t i = 0; i < len;)
		{
			//check Chinese or English word
			if (str[i] > 0)		//English word if (isalnum(str[i]))
			{
				/*CharAlter<1> tmpStr;
				tmpStr.GetChar(str + i);*/
				std::string tmpStr(str + i, 1);

				auto itMapType = cur->children.find(tmpStr);
				if (itMapType == cur->children.end())
				{
					PNode tmpPNode = new Node;
					cur->children.insert(std::pair<std::string, PNode>(tmpStr, tmpPNode));
					pre = cur;
					cur = tmpPNode;
				}
				else
				{
					pre = cur;
					cur = itMapType->second;
				}

				++i;
			}
			else				//Chinese word
			{
				/*CharAlter<CH_SIZE> tmpStr;
				tmpStr.GetChar(str + i);*/
				std::string tmpStr(str + i, CH_SIZE);

				auto itMapType = cur->children.find(tmpStr);
				if (itMapType == cur->children.end())
				{
					PNode tmpPNode = new Node;
					cur->children.insert(std::pair<std::string, PNode>(tmpStr, tmpPNode));
					pre = cur;
					cur = tmpPNode;
				}
				else
				{
					pre = cur;
					cur = itMapType->second;
				}

				i += CH_SIZE;
			}
		}

		++pre->terminalSize;

		return true;
	}

	bool insert(const std::string& str)
	{
		return insert(str.c_str());
	}

	bool find(const char* str)
	{
		if (nullptr == str)
		{
			return false;
		}

		PNode cur = root,pre = nullptr;
		size_t len = strlen(str);

		//int test_time = clock();
		LARGE_INTEGER nFreq, nBegTime, nEndTime;
		double test_time;
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBegTime);
		for (size_t i = 0; i < len;)
		{
			if (str[i] > 0) //if (isalnum(str[i]))
			{
				std::string tmpStr(str + i, 1);

				auto itMap = cur->children.find(tmpStr);
				if (itMap == cur->children.end())
				{
					return false;
				}
				else
				{
					pre = cur;
					cur = itMap->second;
				}

				++i;
			}
			else
			{
				/*CharAlter<CH_SIZE> tmpStr;
				tmpStr.GetChar(str + i);
				printf("3333333333333: %s\n", tmpStr.C_Str()); problem
				tmpStr.GetChar(str + i + CH_SIZE);
				printf("6666666666: %s\n", tmpStr.C_Str());*/
				std::string tmpStr(str + i, CH_SIZE);
				auto itMap = cur->children.find(tmpStr);
				if (itMap == cur->children.end())
				{
					return false;
				}
				else
				{
					pre = cur;
					cur = itMap->second;
				}

				i += CH_SIZE;
			}
		}
		QueryPerformanceCounter(&nEndTime);
		printf("begtime is : %lld\n", nBegTime.QuadPart);
		printf("endtime is : %lld\n", nEndTime.QuadPart);
		test_time = (double)(nEndTime.QuadPart - nBegTime.QuadPart) / (double)nFreq.QuadPart;
		if (pre->terminalSize > 0)
		{
			//printf("frequency is : %lld\n", nFreq);
			printf("searching one word takes : %lf microseconds\n", test_time*1000000);
			return true;
		}

		return false;
	}
	bool find(const std::string& str)
	{
		return find(str.c_str());
	}

	size_t totalNodes(PNode pNode)
	{
		if (nullptr == root)
		{
			return 0;
		}

		size_t size = pNode->terminalSize;
		for (auto it = pNode->children.begin(); it != pNode->children.end();++it)
		{
			size += totalNodes(it->second);
		}

		return size;
	}

private:
	PNode root = new Node;
};





#endif

