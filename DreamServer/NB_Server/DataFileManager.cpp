
#include "DataFileManager.h"
#include "../common/FileParser.h"
#include "../common/Config.h"

using namespace DST;


DataFileManager::DataFileManager()
{
}

DataFileManager::~DataFileManager()
{

}

void DataFileManager::init()
{
	loadAllFiles();
}

bool DataFileManager::loadAllFiles()
{
	if (!loadActivePoint())
	{
		return false;
	}

	if (!loadDirtyWords())
	{
		return false;
	}

	return true;
}

void DataFileManager::initLoadHandlers()
{
	mFileLoadHandlers["ActivePoint"] = &DataFileManager::loadActivePoint;
}

bool DataFileManager::loadActivePoint()
{
	return true;
}


bool DataFileManager::loadDirtyWords()
{
	PARSE_FILE("DirtyWords.csv");

	PARSE_BEGIN;

	std::string strWord = parser.toString("DirtyWord");
	mDirtyWords.push_back(strWord);

	PARSE_END;

	return true;
}
const CStrVec& DataFileManager::getDirtyWords() const
{
	return mDirtyWords;
}