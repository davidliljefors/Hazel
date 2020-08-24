#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include<sstream>

Hazel::Ref<Hazel::Texture2D> logo;

float frametime = 1.f;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.f / 720.f)
{
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
	frametime = ts.GetSeconds();
	HZ_PROFILE_FUNCTION();
	// Update
	m_CameraController.OnUpdate(ts);

	if (Hazel::Input::IsMouseButtonPressed(0))
	{
		std::stringstream ss;
		auto [x, y] = Hazel::Input::GetMousePosition();
		ss << "Mouse X:" << x << ", Y: " << y << std::endl;
	}

	// Render
	Hazel::Renderer2D::ResetStats();

	{
		HZ_PROFILE_SCOPE("Sandbox::Render Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	}

	Hazel::Renderer2D::EndScene();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (float y = -5.f; y < 5.0f; y +=1.0f)
	{
		for (float x = -5.f; x < 5.0f; x += 1.0f)
		{
			glm::vec4 col = { (x + 5.f) / 10.f, (y + 5.f) / 10.f, 0.6f, 0.7f };
			Hazel::Renderer2D::DrawQuad({ x, y, 1.0f }, glm::vec2(0.95f), col);
		}
	}

	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::Text("FPS : %f", 1.f / frametime);

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::DragFloat3("Pos", glm::value_ptr(m_SpritePos), 0.01f);
	ImGui::DragFloat2("Size", glm::value_ptr(m_SpriteSize), 0.01f);

	ImGui::Text("Renderer2D stats : ");
	ImGui::Text("Draw calls : %d", stats.DrawCalls);
	ImGui::Text("Quads : %d", stats.QuadCount);
	ImGui::Text("Vertices : %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices : %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}