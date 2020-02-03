#include <Hazel.h>

class ExampleLayer : public Hazel::Layer 
{
public:
	ExampleLayer()
		:Layer("Example Layer")
	{

	}
	void OnUpdate() override 
	{
		HZ_TRACE("ExampleLayer::Update");
	}

	void OnEvent(Hazel::Event& event)
	{
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}