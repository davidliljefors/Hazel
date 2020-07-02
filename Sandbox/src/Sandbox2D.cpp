#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include<sstream>

using namespace std::chrono;
template <typename Fn>
class Timer
{
public:
	Timer(std::string&& name, Fn&& func)
		: m_Name(name), m_Func(func)
	{
		m_StartTime = high_resolution_clock::now();
	}
	~Timer()
	{
		auto endTime = high_resolution_clock::now();
		auto start = time_point_cast<microseconds>(m_StartTime).time_since_epoch().count();
		auto end = time_point_cast<microseconds>(endTime).time_since_epoch().count();
		auto duration = (end - start) * 0.001;
		m_Func({ m_Name, duration });
	}
private:
	Timer();
	std::string m_Name;
	Fn m_Func;
	time_point<steady_clock> m_StartTime;
};

auto rotation = 1.0f;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.f /720.f)
{

}

void Sandbox2D::OnAttach()
{
	m_Texture = Hazel::Texture2D::Create("assets/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	static int framecount = 0;
	Timer t { "Update", [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); } };
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	if (framecount++ % 1000 == 0)
	{
		HZ_TRACE("Delta time : {0} FPS", (1.f / ts.GetSeconds()));
	}

	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8 }, {0.9f, 0.1f, 0.15f, 1.0f});
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.5f }, { 0.4f, 0.4 }, m_Texture);
	Hazel::Renderer2D::EndScene();

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::SliderAngle("Rotation", &rotation);

	for (auto& result : m_ProfileResults)
	{
		std::string output = result.Name + " %.3fms";
		ImGui::Text(output.c_str(), result.Time);
	}
	m_ProfileResults.clear();
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
