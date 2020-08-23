#pragma once
#include "Hazel.h"
#include "Hazel/Renderer/Texture.h"

class LuaLayer : public Hazel::Layer
{
public:
	LuaLayer();
	virtual ~LuaLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::Texture2D> m_CheckerTexture;
	Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;

	glm::vec3 m_SpritePos{ 1.0f, 1.0f, 1.0f };
	glm::vec2 m_SpriteSize{ 1.0f, 1.0f };

	glm::vec4 m_SquareColor{ 0.2f, 0.8f, 0.3f, 1.f };
};