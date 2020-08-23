#pragma once
#include "Hazel.h"
#include "lua.hpp"
#include "Hazel/Renderer/Texture.h"

struct Thing;

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep ts) override;
	Thing* CreateThing(const char* textureName);

	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	Hazel::OrthographicCameraController m_CameraController;
	std::vector<Thing> m_Things;
	std::vector<Thing> m_ThingsToAdd;
	std::unique_ptr<Thing> m_ControlledThing = nullptr;
public:
	static lua_State* luaInstance;
};