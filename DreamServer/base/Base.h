
#pragma once

#ifndef _BASE_H__
#define _BASE_H__

#include "DataStruct.h"

#define CREATE_SINGLETON(TYPE)			\
static TYPE& getSingleton()				\
{										\
	static TYPE s_##TYPE;				\
	return s_##TYPE;					\
}

#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p) if (p) {delete [] p; p = nullptr; }

//make sure that s is a multiple of 4
#define ALIGN(s) (((s) + 3) & ~3)

class Object
{
public:
	Object() {}
	virtual ~Object() {}
};



#endif