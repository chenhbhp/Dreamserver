

#pragma once
#ifndef _FILE_PARSER_H__
#define _FILE_PARSER_H__

#include "../base/Base.h"

#define CHECK_INDEX(ret)								\
	if (mContent.empty())								\
	{													\
		return ret;										\
	}													\
	if ((mPreIndex+1) > mContent.size())				\
	{													\
		return ret;										\
	}													\
	uint32_t nIndex = 0;								\
	for (; nIndex < mContent[0].size();++nIndex)		\
	{													\
		if (key == mContent[0][nIndex])					\
		{												\
			break;										\
		}												\
	}													\
	if (nIndex >= mContent[mPreIndex + 1].size())		\
	{													\
		return ret;										\
	}


class FileParser:public Object
{
public:
	FileParser(const std::string& filePath);
	FileParser(const char* filePath);
	~FileParser() {}

public:
	bool parse();
	bool parse(const std::string& filePath);

	bool hasNext();

	std::string		toString(const char* key);
	DST::int8_t		toInt8(const char* key);
	DST::int16_t	toInt16(const char* key);
	DST::int32_t	toInt32(const char* key);
	DST::int64_t	toInt64(const char* key);

	DST::uint8_t	toUInt8(const char* key);
	DST::uint16_t	toUInt16(const char* key);
	DST::uint32_t	toUInt32(const char* key);
	DST::uint64_t	toUInt64(const char* key);



private:
	std::string mFile;
	DST::CStrVecVec mContent;

	//split string of line and row
#if WIN32
	char* mLineSplitStr = "\r\n";
#elif linux
	char* mLineSplitStr = "\n";
#endif

	char* mRowSplitStr = ",";

	DST::uint32_t mPreIndex = 0;
	DST::uint32_t mCurIndex = 0;
};




#endif