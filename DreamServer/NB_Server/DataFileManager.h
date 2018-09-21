
#pragma once

#ifndef _DATA_FILE_MANAGER_H__
#define _DATA_FILE_MANAGER_H__

#include "../base/Base.h"

#define PARSE_FILE(FILENAME)											\
std::string curFile(g_Config.curExePath + "data/" + FILENAME);			\
FileParser parser(curFile);												\
if (!parser.parse())													\
{																		\
	printf("%s has some errors !\n", curFile.c_str());					\
	return false;														\
}


#define PARSE_BEGIN			\
while (parser.hasNext())	\
{

#define PARSE_END			\
}





class DataFileManager : public Object
{
public:
	DataFileManager();
	~DataFileManager();
	void init();

public:
	CREATE_SINGLETON(DataFileManager);
	typedef bool (DataFileManager::*LoadHandler)();
	bool loadAllFiles();
public:
	bool loadActivePoint();

	bool loadDirtyWords();
	const DST::CStrVec& getDirtyWords() const;

private:
	DST::CStrVec mDirtyWords;


//use for dynamic update
public:
	void initLoadHandlers();
	void reloadOneFile();
private:
	typedef std::map<std::string, LoadHandler> CStrLoadHandler;
	CStrLoadHandler mFileLoadHandlers;
};




#endif