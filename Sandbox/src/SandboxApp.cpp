#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "imgui/imgui.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.f / 720.f, true)
	{
		m_VertexArray = Hazel::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.3f, 0.0f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));


		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		uint32_t indices[3] = { 0,1,2 };
		indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		//-----------------------

		m_SquareVA = Hazel::VertexArray::Create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

#pragma region Shader
		//------------------------------

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position+1;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0); 
			}		
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position*v_Position, 1.0); 
				color = v_Color;
			}
		)";

		


#pragma endregion Shader

		m_Shader = Hazel::Shader::Create("Shader1", vertexSrc, fragmentSrc);
		auto colorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		auto texShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/Checkerboard.png");
		m_HazelLogoTexture = Hazel::Texture2D::Create("assets/HazelLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(texShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(texShader)->UploadUniformInt("u_Texture", 0);
	}



	void OnUpdate(Hazel::Timestep ts) override
	{
		static int framecount = 0;
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		if (framecount++ % 1000 == 0)
		{
			HZ_TRACE("Delta time : {0} FPS", (1.f / ts.GetSeconds()));
		}

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto colorShader = m_ShaderLibrary.Get("FlatColor");

		std::static_pointer_cast<Hazel::OpenGLShader>(colorShader)->Bind();
		std::static_pointer_cast<Hazel::OpenGLShader>(colorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		{
			Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

			for (size_t i = 0; i < 15; i++)
				for (size_t j = 0; j < 15; j++)
				{
					{
						glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
						glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
						Hazel::Renderer::Submit(colorShader, m_SquareVA, transform);

					}
				}

			auto texShader = m_ShaderLibrary.Get("Texture");
			m_Texture->Bind(0);
			Hazel::Renderer::Submit(texShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			m_HazelLogoTexture->Bind(0);
			Hazel::Renderer::Submit(texShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			// Triangle
			// Hazel::Renderer::Submit(m_Shader, m_VertexArray);
			Hazel::Renderer::EndScene();
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& e) override
	{
		m_CameraController.OnEvent(e);

		if (e.GetEventType() == Hazel::EventType::WindowResize)
		{
			auto& re = static_cast<Hazel::WindowResizeEvent&>(e);
			float zoom = re.GetWidth() / 1280.f;
			m_CameraController.SetZoomLevel(zoom);
		}
	}

private:
	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Texture2D> m_HazelLogoTexture;

	Hazel::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor { 0.2f, 0.8f, 0.3f };
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}


