#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include<sstream>

Hazel::Ref<Hazel::Texture2D> logo;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.f / 720.f)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerTexture = Hazel::Texture2D::Create("assets/Checkerboard.png");
	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/rpg_spritesheet.png");
	logo = Hazel::Texture2D::Create("assets/HazelLogo.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	static int framecount = 0;
	HZ_PROFILE_FUNCTION();
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Hazel::Renderer2D::ResetStats();
	if (framecount++ % 1000 == 0)
	{
		HZ_TRACE("Delta time : {0} FPS", (1.f / ts.GetSeconds()));
	}

	{
		HZ_PROFILE_SCOPE("Sandbox::Render Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	}

	{
		HZ_PROFILE_SCOPE("Sandobx::Render Draw");
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8 }, { 0.9f, 0.1f, 0.15f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f , -0.1f }, { 20.0f, 20.0f }, m_CheckerTexture, 10.f);

		Hazel::Renderer2D::DrawQuad(m_SpritePos, m_SpriteSize, m_SpriteSheet);



		static float rot = 0;
		rot += 50.f * ts;
		Hazel::Renderer2D::DrawRotatedQuad({ -2.0f, -2.0f }, { 3.f, 3.f }, rot, logo);
			
	}
	Hazel::Renderer2D::EndScene();


	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (float y = -5.f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 col = { (x + 5.f) / 10.f, (y + 5.f) / 10.f, 0.6f, 0.7f };
			Hazel::Renderer2D::DrawQuad({ x, y }, glm::vec2(0.45f), col);
		}
	}
	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::DragFloat3("Pos", glm::value_ptr(m_SpritePos), 0.01f);
	ImGui::DragFloat2("Size", glm::value_ptr(m_SpriteSize), 0.01f);

	ImGui::Text("Renderer2D stats : ");
	ImGui::Text("Draw calls : %d", stats.DrawCalls);
	ImGui::Text("Quads : %d", stats.QuadCount);
	ImGui::Text("Vertices : %d", stats.GetVertexCount());
	ImGui::Text("Indices : %d", stats.GetIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
