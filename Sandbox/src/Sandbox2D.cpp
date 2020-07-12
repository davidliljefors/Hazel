#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include<sstream>

auto rotation = 1.0f;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.f / 720.f)
{

}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();
	m_Texture = Hazel::Texture2D::Create("assets/Checkerboard.png");
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
		Hazel::Renderer2D::DrawQuad({ -5.0f, -5.0f }, { 10.f, 10.f }, m_Texture, 10.f);
	}
	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
