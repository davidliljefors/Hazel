#pragma once

#include "LuaLayer.h"
#include "lua.hpp"
#include "Thing.h"

// createThing(Sandbox2D* app, char* textureFilename, pos x, pos y)
// returns c++ owned pointer to created thing
int wrap_CreateThing(lua_State* L)
{
	if (lua_gettop(L) != 4) return -1;

	LuaLayer* app = static_cast<LuaLayer*>(lua_touserdata(L, -4));
	auto filepath = lua_tostring(L, -3);
	auto x = lua_tonumber(L, -2);
	auto y = lua_tonumber(L, -1);
	auto thing = app->CreateThing(filepath);
	thing->SetPos((float)x, (float)y);
	return 0;
}

// destroyThing(Thing* thing)
int wrap_DestroyThing(lua_State* L)
{
	if (lua_gettop(L) != 1) return -1;
	
	Thing* thing = static_cast<Thing*>(lua_touserdata(L, -1));
	thing->Kill();
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