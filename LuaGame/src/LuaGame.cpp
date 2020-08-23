#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LuaLayer.h"

class LuaGame : public Hazel::Application
{
public:
	LuaGame()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new LuaLayer());
	}

	~LuaGame()
	{
	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new LuaGame();
}


