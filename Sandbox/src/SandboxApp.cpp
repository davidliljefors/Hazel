#include <Hazel.h>
#include <future>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include "imgui/imgui.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>



class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-3.2f, 3.2f, -1.8f, 1.8f), m_CameraPosition(0.0f)
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

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position+1;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0); 
			}		
		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0); 
			}
		)";

		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0); 
			}		
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord); 
			}
		)";

#pragma endregion Shader

		m_Shader = Hazel::Shader::Create(vertexSrc, fragmentSrc);
		m_FlatColorShader = Hazel::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc);
		m_TextureShader = Hazel::Shader::Create(textureVertexSrc, textureFragmentSrc);

		m_Texture = Hazel::Texture2D::Create("assets/Checkerboard.png");
		m_HazelLogoTexture = Hazel::Texture2D::Create("assets/HazelLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}



	void OnUpdate(Hazel::Timestep ts) override
	{
		static int framecount = 0;		

		if (Hazel::Input::IsKeyPressed(HZ_KEY_KP_ADD))
		{
			m_Camera.SetZoom(1.01f);
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_KP_SUBTRACT))
		{
			m_Camera.SetZoom(0.99f);
		}

		if (framecount++ % 1000 == 0)
		{
			HZ_TRACE("Delta time : {0} FPS", (1.f / ts.GetSeconds()));
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		{
			Hazel::Renderer::BeginScene(m_Camera);

			for (size_t i = 0; i < 15; i++)
				for (size_t j = 0; j < 15; j++)
				{
					{
						glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
						glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
						Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);

					}
				}

			m_Texture->Bind(0);
			Hazel::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			m_HazelLogoTexture->Bind(0);
			Hazel::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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

	void OnEvent(Hazel::Event& event) override
	{
	}

private:
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::Shader> m_TextureShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Texture2D> m_HazelLogoTexture;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 50.0f;
	float m_CameraMoveSpeed = 1.0f;
	glm::vec3 m_SquareColor = { 0.2f, 0.8f, 0.3f };
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}


