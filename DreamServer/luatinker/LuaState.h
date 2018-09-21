
#pragma once

#ifndef _LUA_STATE_H__
#define _LUA_STATE_H__

#include <Base.h>
#include "lua_tinker.h"

class LuaState : public Object
{
public:
	LuaState();
	~LuaState();

	void init(const std::string& filename);

private:
	lua_State* mL = lua_open();
};




#endif