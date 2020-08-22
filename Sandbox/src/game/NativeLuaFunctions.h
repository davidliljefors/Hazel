#pragma once

#include "lua.hpp"
#include "Thing.h"

int wrap_DestroyThing(lua_State* L)
{
	if (lua_gettop(L) != 1) return -1;
	{
		Thing* thing = static_cast<Thing*>(lua_touserdata(L, -1));
		thing->Kill();
	}
	return 0;
}

//
//int wrap_MoveSprite(lua_State* L)
//{
//	if (lua_gettop(L) != 3) return -1;
//	Sprite* spr = static_cast<Sprite*>(lua_touserdata(L, -3));
//	lua_Number newX = lua_tonumber(L, -2);
//	lua_Number newY = lua_tonumber(L, -1);
//	spr->SetPos({ (float)newX, (float)newY, 0.f });
//	return 0;
//}