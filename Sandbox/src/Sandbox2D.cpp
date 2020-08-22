#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>

#include "LuaUtils.h"
#include "game/Sprite.h"
#include "ImageTools.h"

#include "game/NativeLuaFunctions.h"

char luaInputBuffer[256];

lua_State* Sandbox2D::luaInstance = luaL_newstate();

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.f / 720.f)
{
	m_Things.reserve(256);
	luaL_openlibs(luaInstance);
	lua_pushlightuserdata(luaInstance, this);
	lua_setglobal(luaInstance, "game");
	lua_register(luaInstance, "destroyThing", wrap_DestroyThing);
	LuaUtils::CheckError(luaInstance, luaL_dofile(luaInstance, "behaviour.lua"));

	m_Things.emplace_back(Hazel::Texture2D::Create("zerotwo.png"));
	m_Things.emplace_back(Hazel::Texture2D::Create("zerotwo.png"));
	m_Things.emplace_back(Hazel::Texture2D::Create("zerotwo.png"));
	m_Things.emplace_back(Hazel::Texture2D::Create("zerotwo.png"));
	m_Things.emplace_back(Hazel::Texture2D::Create("zerotwo.png"));
	m_ControlledThing = std::make_unique<Thing>(Hazel::Texture2D::Create("zerotwo.png"));
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnDetach()
{

}

bool bPrssedLastFrame = false;
float movespeed = 1.f;

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	if (m_ControlledThing)
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		{
			m_ControlledThing->Move({ -movespeed * ts, 0 });
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		{
			m_ControlledThing->Move({ movespeed * ts, 0 });
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		{
			m_ControlledThing->Move({ 0, movespeed * ts });
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		{
			m_ControlledThing->Move({0, -movespeed * ts});
		}
	}

	for (auto& t : m_Things)
	{
		if (m_ControlledThing)
		{
			if (m_ControlledThing->Intersects(&t))
			{
				m_ControlledThing->OnCollision(&t);
			}
		}
	}

	if (Hazel::Input::IsMouseButtonPressed(0) && !bPrssedLastFrame)
	{
		bPrssedLastFrame = true;
		auto mousepos = Hazel::Input::GetMousePosition();
		auto mouseworld = m_CameraController.GetMouseWorldPosition({ mousepos.first, mousepos.second });

	}
	
	if (!Hazel::Input::IsMouseButtonPressed(0) && bPrssedLastFrame)
	{
		bPrssedLastFrame = false;
	}

	// Render
	Hazel::Renderer2D::ResetStats();
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::Clear();
	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (const auto& thing : m_Things)
	{
		thing.Render();
	}
	if (m_ControlledThing) m_ControlledThing->Render();
	Hazel::Renderer2D::EndScene();

	std::erase_if(m_Things, [](const Thing& thing)
	{
		return thing.IsDead();
	});
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Lua interpreter");
	ImGui::Text("Enter lua code:");
	if (ImGui::InputText("", luaInputBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		LuaUtils::CheckError(luaInstance, luaL_dostring(luaInstance, luaInputBuffer));
		memset(luaInputBuffer, 0, 256);
	}
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}