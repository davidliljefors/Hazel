#pragma once

#include <string>
#include <iostream>
#include "lua.hpp"

namespace LuaUtils
{
#pragma region Pushing Values
	void _push(lua_State* L, int arg)
	{
		lua_pushinteger(L, arg);
	}

	void _push(lua_State* L, double arg)
	{
		lua_pushnumber(L, arg);
	}

	void _push(lua_State* L, float arg)
	{
		lua_pushnumber(L, arg);
	}

	void _push(lua_State* L, const char* arg)
	{
		lua_pushstring(L, arg);
	}

	void _push(lua_State* L, bool arg)
	{
		lua_pushboolean(L, arg);
	}

	void _push(lua_State* L, void* arg)
	{
		lua_pushlightuserdata(L, arg);
	}

	template<typename T>
	void Push(lua_State* L, T&& arg)
	{
		return _push(L, std::forward<T>(arg));
	}

	template <typename... Args>
	void PushAll(lua_State* L, Args&&... args) {
		(Push(L, args), ...);
	}

	template<typename... Args>
	int Call(lua_State* L,  char* funcName, Args&&... args)
	{
		lua_getglobal(L, funcName);
		if (lua_isfunction(L, -1))
		{
			LuaUtils::PushAll(L, args...);
			if (CheckError(L, lua_pcall(L, sizeof...(Args), 0, 0)))
			{
				return 0;
			}
			return 1;
		}
		std::cerr << "Unable to find lua function \"" << funcName << "\"\n";
		return 0;
	}

	bool CheckError(lua_State* L, int pcallReturnValue)
	{
		if (pcallReturnValue != LUA_OK)
		{
			std::string errormsg = lua_tostring(L, -1);
			std::cerr << errormsg << '\n';
			return false;
		}
		return true;
	};
}