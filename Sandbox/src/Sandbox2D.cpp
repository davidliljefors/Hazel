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

	/// ------ DOCKSPACE

	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Exit"))
			{
				Hazel::Application::Get().Close();
			}
			
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}