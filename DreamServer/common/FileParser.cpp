
#include <fstream>
#include "FileParser.h"

using namespace DST;

FileParser::FileParser(const std::string& filePath)
	:
	mFile(filePath)
{

}

FileParser::FileParser(const char* filePath)
	:
	mFile(filePath)
{

}

bool FileParser::parse()
{
	std::ifstream ifs;
	ifs.open(mFile.c_str(), std::ifstream::binary);

	if (!ifs)
	{
		ifs.close();
		return false;
	}

	//get length of file
	ifs.seekg(0, ifs.end);
	uint32_t length = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	
	//copy content of file
	char* contentBuffer = new char[length + 5]; //more 5 for checking end of file
	memset(contentBuffer, 0, length + 5);
	ifs.read(contentBuffer, length);
	ifs.close();

	//check \r\n or \n
	char* foundTag = strstr(contentBuffer, mLineSplitStr);
	if (nullptr == foundTag)
	{
		mLineSplitStr = "\n";
		foundTag = strstr(contentBuffer, mLineSplitStr);
		if (nullptr == foundTag)
		{
			//log_error
			return false;
		}
	}

	char* line_beg = contentBuffer;
	char* line_end = nullptr;
	size_t LSplitLen = strlen(mLineSplitStr);
	size_t line_num = 0;
	CStrVec record;
	while (line_beg)
	{
		++line_num;
		record.clear();
		line_end = strstr(line_beg, mLineSplitStr);
		if (nullptr == line_end)
		{
			break;
		}

		std::string line_content(line_beg, line_end - line_beg);

		if (!line_content.empty() || line_num != 2) //avoid the second line "UINT16,UINT16,STRING,..."
		{
			size_t begTag = 0;
			size_t endTag = line_content.find(mRowSplitStr, begTag);

			while (true)
			{
				if (std::string::npos == endTag)
				{
					record.push_back(line_content.substr(begTag));
					break;
				}

				record.push_back(line_content.substr(begTag,endTag-begTag));

				begTag = endTag + 1;
				endTag = line_content.find(mRowSplitStr, begTag);
			}

			mContent.push_back(record);
		}

		line_beg = line_end + LSplitLen;
	}

	delete[] contentBuffer;
	contentBuffer = nullptr;

	return true;
}

bool FileParser::parse(const std::string& filePath)
{
	mFile = filePath;
	return parse();
}

bool FileParser::hasNext()
{
	mPreIndex = mCurIndex;
	if (mCurIndex + 1 >= mContent.size())
	{
		return false;
	}

	++mCurIndex;
	return true;
}

std::string FileParser::toString(const char* key)
{
	CHECK_INDEX("");
	return mContent[mPreIndex + 1][nIndex];
}

int8_t FileParser::toInt8(const char* key)
{
	CHECK_INDEX(0);
	return atoi(mContent[mPreIndex + 1][nIndex].c_str());
}

uint8_t FileParser::toUInt8(const char* key)
{
	CHECK_INDEX(0);
	return atoi(mContent[mPreIndex + 1][nIndex].c_str());
}

int16_t FileParser::toInt16(const char* key)
{
	CHECK_INDEX(0);
	return atoi(mContent[mPreIndex + 1][nIndex].c_str());
}

uint16_t FileParser::toUInt16(const char* key)
{
	CHECK_INDEX(0);
	return atoi(mContent[mPreIndex + 1][nIndex].c_str());
}

int32_t FileParser::toInt32(const char* key)
{
	CHECK_INDEX(0);
	return atoi(mContent[mPreIndex + 1][nIndex].c_str());
}

uint32_t FileParser::toUInt32(const char* key)
{
	CHECK_INDEX(0);
	return strtoul(mContent[mPreIndex + 1][nIndex].c_str(), nullptr, 0);
}

int64_t FileParser::toInt64(const char* key)
{
	CHECK_INDEX(0);
	return strtoll(mContent[mPreIndex + 1][nIndex].c_str(), nullptr, 0);
}

uint64_t FileParser::toUInt64(const char* key)
{
	CHECK_INDEX(0);
	return strtoull(mContent[mPreIndex + 1][nIndex].c_str(), nullptr, 0);
}

