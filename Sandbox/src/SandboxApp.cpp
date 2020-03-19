#include <Hazel.h>

#include "imgui/imgui.h"
#include <glm/gtx/string_cast.hpp>

glm::mat4 model(1.f);

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-3.2f, 3.2f, -1.8f, 1.8f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.3f, 0.0f, 1.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));


		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		uint32_t indices[3] = { 0,1,2 };
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		//-----------------------

		m_SquareVA.reset(Hazel::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			 -0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

#pragma region Shader
		//------------------------------

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_ModelView;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position+1;
				v_Color = a_Color;
				gl_Position = u_ModelView * u_ViewProjection * vec4(a_Position, 1.0); 
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

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position+1;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0); 
			}		
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position*v_Position, 1.0); 
			}
		)";
#pragma endregion Shader

		m_Shader = std::make_unique<Hazel::Shader>(vertexSrc, fragmentSrc);

		m_Shader2 = std::make_unique<Hazel::Shader>(vertexSrc2, fragmentSrc2);
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		HZ_TRACE("Delta time : {0}ms", ts.GetMilliseconds());
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{ model[0][3] += 1.0f * ts; };
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{ model[0][3] -= 1.0f * ts; };


		if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
		{ model[1][3] += 1.0f * ts; };
		if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
		{ model[1][3] -= 1.0f * ts; };

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		{ m_CameraPosition.x -= m_CameraMoveSpeed * ts; };
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		{ m_CameraPosition.x += m_CameraMoveSpeed * ts; };

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		{ m_CameraPosition.y += m_CameraMoveSpeed * ts; };
		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		{ m_CameraPosition.y -= m_CameraMoveSpeed * ts; };




		m_Shader->UploadUniformMat4("u_ModelView", glm::transpose(model));

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		{
			Hazel::Renderer::BeginScene(m_Camera);

			Hazel::Renderer::Submit(m_Shader2, m_SquareVA);
			Hazel::Renderer::Submit(m_Shader, m_VertexArray);

			Hazel::Renderer::EndScene();
		}
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(Hazel::Event& event) override
	{
	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_Shader2;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 1.0f;
	float m_CameraMoveSpeed = 1.0f;
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