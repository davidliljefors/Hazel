#pragma once
#include "Hazel.h"
#include "lua.hpp"
#include "Hazel/Renderer/Texture.h"

struct Thing;

class LuaLayer : public Hazel::Layer
{
public:
	LuaLayer();
	virtual ~LuaLayer() = default;

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