#pragma once
#include <Hazel.h>
#include "LuaLayer.h"
#include "LuaUtils.h"
#include "lua.hpp"


#include "Collider.h"
#include "Sprite.h"

#include <cmath>

static float counter = 0;
// A thing is a thing in the game. Behaviour is in its lua script
struct Thing
{
	inline Thing(Hazel::Ref<Hazel::Texture2D> texture)
		: m_Texture(texture)
	{
		m_Pos.x = counter;
		m_Pos.y = 0;
		counter += 1.f;
	}

	inline bool Intersects(float xp, float yp)
	{
		return  (xp >= m_Pos.x && xp <= (m_Pos.x + m_Size.x) &&
			yp >= m_Pos.y && yp <= (m_Pos.y + m_Size.y));
	}

	// Simple AABB
	inline bool Intersects(Thing* other)
	{
		if (
			this->m_Pos.x < other->m_Pos.x + other->m_Size.x &&
			this->m_Pos.x + this->m_Size.x > other->m_Pos.x &&
			this->m_Pos.y < other->m_Pos.y + other->m_Size.y &&
			this->m_Pos.y + this->m_Size.y > other->m_Pos.y
			)
			return true;
		return false;
	}

	inline void OnCollision(Thing* other)
	{
		lua_State* L = LuaLayer::luaInstance;
		LuaUtils::Call(L, "onCollision", this, other);
	}

	inline void Render() const
	{
		Hazel::Renderer2D::DrawQuad(m_Pos, { 1.0f,1.0f }, m_Texture);
	}

	inline auto GetPos() const
	{
		return m_Pos;
	}

	inline void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}

	inline void SetPos(glm::vec2 newpos)
	{
		m_Pos = newpos;
	}
	inline void Move(glm::vec2 delta)
	{
		m_Pos += delta;
	}

	inline void Kill()
	{
		dead = true;
	}

	inline bool IsDead() const
	{
		return dead;
	}
	glm::vec2 m_Pos {0.f, 0.f};
	glm::vec2 m_Size{ 1.0f, 1.0f };

private:
	bool dead = false;
	Hazel::Ref<Hazel::Texture2D> m_Texture = nullptr;
	std::unique_ptr<Collider> m_Collider = nullptr;
};