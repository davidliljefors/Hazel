#pragma once
#include <Hazel.h>

#include <glm\glm.hpp>

class Sprite
{
public:
	Sprite() = default;
	Sprite(float x, float y, Hazel::Ref<Hazel::Texture2D> texture)
	{
		this->x = x;
		this->y = y;
		this->texture = texture;
	}


	inline void Draw() const
	{
		Hazel::Renderer2D::DrawQuad({x+0.5f, y+0.5f}, size, texture);
	}

	inline bool Intersects(float xp, float yp)
	{
		return  (	xp >= x && xp <= (x + size.x) &&
					yp >= y && yp <= (y + size.y)	);
	}

	inline void SetPos(glm::vec3 position)
	{
		this->x = position.x;
		this->y = position.y;
	}

	float x;
	float y;
private:
	glm::vec2 size { 1.0f, 1.0f };
	Hazel::Ref<Hazel::Texture2D> texture;
};