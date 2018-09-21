#include "LuaState.h"

LuaState::LuaState()
{
	luaopen_base(mL);
}

LuaState::~LuaState()
{
	lua_close(mL);
}

void LuaState::init(const std::string& filename)
{
	lua_tinker::dofile(mL, filename.c_str());
}