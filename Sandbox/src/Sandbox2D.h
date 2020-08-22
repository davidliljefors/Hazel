#pragma once
#include "Hazel.h"
#include "lua.hpp"
#include "Hazel/Renderer/Texture.h"
#include "Sprite.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
	Sprite* CreateSprite(const std::string& filename);

private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::Texture2D> m_CheckerTexture;
	glm::vec4 m_SquareColor { 0.2f, 0.8f, 0.3f, 1.f };
	std::vector<Sprite> m_UserSprites;

	lua_State* L = nullptr;
};