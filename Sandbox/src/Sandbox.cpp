//Inherits the Hazel Application and define how to create application

#include <Hazel.h>

#include "imgui.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		:Layer("Example Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.2f, 0.7f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.7f, 0.2f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.1f, 0.7f, 1.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float4, "a_Color"}
			});
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[] = {
			0, 1, 2,
		};

		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);



		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec4 v_color;

			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Position , 1.0);
				v_color = a_Color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			out vec4 FragColor;
			
			in vec4 v_color;

			void main()
			{
				FragColor = v_color;
			}
		)";

		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

		//square
		m_SquareVertexArray.reset(Hazel::VertexArray::Create());

		float squarevertices[] = {
			-0.7f, -0.7f, 0.0f,
			 0.7f, -0.7f, 0.0f,
			 0.7f,  0.7f, 0.0f,
			-0.7f,  0.7f, 0.0f,
		};

		std::shared_ptr<Hazel::VertexBuffer> squarevertexBuffer;
		squarevertexBuffer.reset(Hazel::VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

		squarevertexBuffer->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"},
			});
		m_SquareVertexArray->AddVertexBuffer(squarevertexBuffer);


		uint32_t squareindices[] = {
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<Hazel::IndexBuffer> squareindexBuffer;
		squareindexBuffer.reset(Hazel::IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareindexBuffer);


		std::string squarevertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;

			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Position , 1.0);
			}
		)";

		std::string squarefragmentSrc = R"(
			#version 330 core
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = vec4(0.2f, 0.1f, 0.7f, 1.0f);
			}
		)";

		m_SquareShader.reset(new Hazel::Shader(squarevertexSrc, squarefragmentSrc));

	}

	~ExampleLayer()
	{

	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		HZ_CLIENT_TRACE("Timestep: {0}s, {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
			m_CameraRotation += m_CameraRotateSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
			m_CameraRotation -= m_CameraRotateSpeed * ts;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_SquareShader, m_SquareVertexArray);
		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		
	}

	void OnEvent(Hazel::Event& event) override
	{

	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_SquareShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotateSpeed = 180.0f;
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

//implement how to create app
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}