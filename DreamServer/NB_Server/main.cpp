#include <iostream>
#include <string>
#include <IOCP.h>
#include <Trie.h>
#include <WTrie.h>
#include <Config.h>
#include <MapTrie.h>
#include <FileParser.h>
//#include <lua51/lua_tinker.h>
//#include <boost/thread.hpp>
//#include <boost/bind.hpp>
#include <Utils.h>
#include <BSTree.h>
#include <RingBuffer.h>

//#include <luabind/luabind.hpp>


#include "DataFileManager.h"
#include "Bag.h"

#include "HashTable.h"
#include <ReportDump.h>
#include <Queue.h>
#include <MsgProcessor.h>

#include <random>



/***********************************************************************
*global    variables
************************************************************************/

Global_Config g_Config;


std::vector<PPER_HANDLER_DATA> g_ClientGroup;
HANDLE g_Mutex = CreateMutex(nullptr, false, nullptr);

/************************************************************************/

std::string getCurPath(char* inPath)
{
	//get current exepath
	std::string appPath = inPath;
	std::string str;
	char buf[128] = { 0 };

	size_t i = appPath.rfind('\\');
	if (i != std::string::npos)
	{
		appPath.copy(buf, i, 0);
		appPath = buf;
	}

	char bufDes[128] = { 0 };
	size_t j = appPath.rfind('\\');
	if (j != std::string::npos)
	{
		appPath.copy(bufDes, j + 1, 0);
		appPath = bufDes;
	}

	std::string::size_type pos = 0;
	while ((pos = appPath.find('\\', pos)) != std::string::npos)
	{
		appPath.replace(pos, 1, "/");
		++pos;
	}

	std::cout << "current apppath: " << appPath.c_str() << std::endl;

	return appPath;
}




int main(int argc, char* argv[])
{
	std::cout << "This is NB Server!" << std::endl;
	srand(time(nullptr));

	std::cout << Utils::start_of_day() << std::endl;

	std::default_random_engine generator(9);
	std::uniform_int_distribution<int> distrib(0, 9);

	int loopNum = 30000;
	std::map<int, int> randMap;
	while (loopNum-- > 0)
	{
		int randNum = distrib(generator);
		//int temp = randMap[randNum];
		++randMap[randNum] ;
	}

	for (auto it = randMap.begin(); it != randMap.end(); ++it)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}

	/*int loopNum = 3000;
	int N = 8;
	std::map<int, int> randMap;
	while (loopNum-- > 0)
	{
	int randNum = rand() % N;
	int temp = randMap[randNum];
	randMap[randNum] = ++temp;
	}

	for (auto it = randMap.begin(); it != randMap.end();++it)
	{
	std::cout << it->first << ": " << it->second << std::endl;
	}
	*/
	//HashTable<DST::uint32_t> hashTable;

	/*int a[] = {100,16,20,3,49,94,87,2,1,76,35,32,23,10,2,0,43,25,31,27,24,17,19,5,9,65};
	int num = sizeof(a) / sizeof(a[0]);
	Utils::heapSort(a, num);
	for (int i = 0; i < num; ++i)
	{
	std::cout << a[i] << ",";
	}
	std::cout << std::endl;*/

	//RingBuffer<std::string> rb;
	/*int a[10] = {4, 12, 8, 7, 3, 45, 23, 5, 6, 9};
	Utils::quickSort(a, 0, 9);
	printf("%d\n", a[0]);*/

	/*BSTree::BSTree<DST::uint32_t> g_BSTree(1, 10);
	g_BSTree.insert(2, 20, 30);
	g_BSTree.insert(3, 20, 30);
	g_BSTree.insert(4, 70, 30);
	g_BSTree.insert(4, 70, 40);
	g_BSTree.insert(3, 50, 30);

	std::vector<BSTree::BSTNodeData<DST::uint32_t>> dataList;
	g_BSTree.getTopN(dataList, 2);

	g_BSTree.change(2, 5, 20, 30);
	g_BSTree.getTopN(dataList, 2);

	printf("/////////////////  rank is : %d\n", g_BSTree.getRank(2, 20, 30));*/

	/*char buf[20];
	const char* splitStr = "%*[^_]_%[^.]";
	sscanf("Headportrait_1.png", splitStr, buf);

	printf("%s\n", buf);*/

	/*DST::AnyData a((int*)0);
	a.getRealData<int>();*/

	/*g_Config.curExePath = getCurPath(*argv);
	DataFileManager::getSingleton().init();

	MapTrie<MapTrieNode> mt;
	MapTrie<HashMapTrieNode> hmt;

	const DST::CStrVec& dirtyWords = DataFileManager::getSingleton().getDirtyWords();
	for (auto it = dirtyWords.begin(); it != dirtyWords.end(); ++it)
	{
	mt.insert(*it);
	hmt.insert(*it);
	}

	printf("map find result: %d\n", mt.find("±©ÔêµÄ¾¯Ðl±øì`»ê"));
	printf("hashmap find result: %d\n", hmt.find("±©ÔêµÄ¾¯Ðl±øì`»ê"));*/

	/*uint32_t randSeed = 0;
	srand(100);
	for (; randSeed <= 300;++randSeed)
	{
	srand(100);
	uint32_t a = rand();
	uint32_t b = rand();
	uint32_t c = rand();

	std::cout << a << std::endl;
	<< b << std::endl
	<< c << std::endl << std::endl;
	}*/


	
/***** test for WTrie
	WTrie<TRIE_SIZE, WIndexType<TRIE_SIZE>> wt;
	wt.insert(L"³ÂÎÄ");
	wt.insert(L"chb");
	wt.insert(L"³Âº£²¨");


	std::cout << "total nodes is: " << wt.totalNodes(wt.root) << std::endl;
	std::cout << "find chb : " << wt.find(L"chb")<<std::endl;
	std::cout << "find haibo: " << wt.find(L"³Âº£²¨") << std::endl;*/
	/*t.erase("hi");
	std::cout << "total nodes is: " << t.totalNodes(t.root) << std::endl;*/

	/*MapTrie<MapTrieNode> mt;
	mt.insert("³Âsº£²¨");
	mt.insert("chb");
	mt.insert("hello");
	mt.insert("world");

	std::cout << "total nodes is: " << mt.totalNodes(mt.getRoot()) << std::endl;
	printf("find result: %d\n", mt.find("³Âsº£²¨"));
	printf("find result: %d\n", mt.find("chb"));*/

	/*std::string curAppPath(getCurPath(*argv) + "data/ActivePoint.csv");
	std::cout << "current apppath: " << curAppPath.c_str() << std::endl;
	FileParser fp(curAppPath.c_str());
	if (!fp.parse())
	{
	std::cout << "parsing file has some errors ! " << std::endl;
	}
	else
	std::cout << "parsing file has finished !" << std::endl;*/


	return 0;
}