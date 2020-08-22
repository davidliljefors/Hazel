#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>

#include "LuaUtils.h"
#include "Sprite.h"
#include "ImageTools.h"

Hazel::Ref<Hazel::Texture2D> logo;

int wrap_CreateSprite(lua_State* L)
{
	if (lua_gettop(L) != 2) return -1;
	Sandbox2D* sbox = static_cast<Sandbox2D*>(lua_touserdata(L, 1));
	const char* filepath = lua_tostring(L, 2);
	lua_pushlightuserdata(L, sbox->CreateSprite(filepath));
	return 1;
}

int wrap_MoveSprite(lua_State* L)
{
	if (lua_gettop(L) != 3) return -1;
	Sprite* spr = static_cast<Sprite*>(lua_touserdata(L, -3));
	lua_Number newX = lua_tonumber(L, -2);
	lua_Number newY = lua_tonumber(L, -1);
	spr->SetPos({ (float)newX, (float)newY, 0.f });
	return 0;
}

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.f / 720.f)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	lua_pushlightuserdata(L, this);
	lua_setglobal(L, "sandbox");
	lua_register(L, "_CreateSprite", wrap_CreateSprite);
	lua_register(L, "_MoveSprite", wrap_MoveSprite);
	LuaUtils::CheckError(L, luaL_dofile(L, "behaviour.lua"));

}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerTexture = Hazel::Texture2D::Create("assets/Checkerboard.png");
	logo = Hazel::Texture2D::Create("assets/HazelLogo.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);
	
	static bool bPrssedLastFrame = false;

	if (Hazel::Input::IsMouseButtonPressed(0) && !bPrssedLastFrame)
	{
		bPrssedLastFrame = true;
		auto mousepos = Hazel::Input::GetMousePosition();
		auto mouseworld = m_CameraController.GetMouseWorldPosition({ mousepos.first, mousepos.second });
		for (auto& s : m_UserSprites)
		{
			if (s.Intersects(mouseworld.x, mouseworld.y))
			{
				LuaUtils::Call(L, "onSpriteClicked", &s, s.x, s.y);
			}
		}
	}
	if (!Hazel::Input::IsMouseButtonPressed(0) && bPrssedLastFrame)
	{
		bPrssedLastFrame = false;
	}

	// Render
	Hazel::Renderer2D::ResetStats();

	{
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	}

	{

		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8 }, { 0.9f, 0.1f, 0.15f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f , -0.1f }, { 20.0f, 20.0f }, m_CheckerTexture, 10.f);
		Hazel::Renderer2D::DrawQuad({ -2.0f, -2.0f }, { 3.f, 3.f }, logo);

		for (auto& s : m_UserSprites)
		{
			s.Draw();
		}
	}
	Hazel::Renderer2D::EndScene();

}

char luaInputBuffer[256];
void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Lua interpreter");
	ImGui::Text("Enter lua code:");
	if (ImGui::InputText("", luaInputBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		LuaUtils::CheckError(L, luaL_dostring(L, luaInputBuffer));
		memset(luaInputBuffer, 0, 256);
	}
	ImGui::End();

	//ImGui::Begin("Settings");
	//auto stats = Hazel::Renderer2D::GetStats();
	//ImGui::Text("Renderer2D stats : ");
	//ImGui::Text("Draw calls : %d", stats.DrawCalls);
	//ImGui::Text("Quads : %d", stats.QuadCount);
	//ImGui::Text("Vertices : %d", stats.GetVertexCount());
	//ImGui::Text("Indices : %d", stats.GetIndexCount());

	//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	//ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}

Sprite* Sandbox2D::CreateSprite(const std::string& filename)
{
	auto texture = Hazel::Texture2D::Create(filename);
	auto spr = m_UserSprites.emplace_back(0.0f,0.0f,texture);
	return &spr;
}


//void Sandbox2D::SaveMapCompressed()
//{
//	HZ_TRACE("Save map compressed");
//	std::ofstream ofs(inputfilename, std::ofstream::binary);
//	ofs.write(reinterpret_cast<char*>(&mapdata), sizeof(mapdata));
//	bool inRleSegment = false;
//	uint16_t rleCount = 0;
//	for (auto& tile : worldTiles)
//	{
//		if (tile.GetColor() == 0) // if black pixel, start rle segment
//		{
//			inRleSegment = true;
//			rleCount++;
//		}
//		else
//		{
//			if (inRleSegment)
//			{
//				int val = 0;
//				ofs.write(reinterpret_cast<char*>(&val), 3);
//				ofs.write(reinterpret_cast<char*>(&rleCount), sizeof(rleCount));
//				ofs.write(reinterpret_cast<char*>(tile.GetColorPtr()), 3);
//				inRleSegment = false;
//				rleCount = 0;
//			}
//			else
//			{
//				ofs.write(reinterpret_cast<char*>(tile.GetColorPtr()), 3);
//			}
//		}
//	}
//	if (inRleSegment)
//	{
//		int val = 0;
//		ofs.write(reinterpret_cast<char*>(&val), 3);
//		ofs.write(reinterpret_cast<char*>(&rleCount), sizeof(rleCount));
//	}
//}

