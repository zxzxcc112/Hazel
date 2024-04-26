//Inherits the Hazel Application and define how to create application

#include <Hazel.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		:Layer("Example Layer"), m_OrthographicCameraController(1280.0f / 720.0f, true)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.2f, 0.7f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.7f, 0.2f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.1f, 0.6f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float4, "a_Color"}
			});
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[] = {
			0, 1, 2,
		};

		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);



		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_color;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position , 1.0);
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

		m_Shader = Hazel::Shader::Create("TriangleColorPos", vertexSrc, fragmentSrc);

		//square
		m_SquareVertexArray.reset(Hazel::VertexArray::Create());

		float squarevertices[] = {
			-0.7f, -0.7f, 0.0f, 0.0f, 0.0f,
			 0.7f, -0.7f, 0.0f, 1.0f, 0.0f,
			 0.7f,  0.7f, 0.0f, 1.0f, 1.0f,
			-0.7f,  0.7f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squarevertexBuffer;
		squarevertexBuffer.reset(Hazel::VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

		squarevertexBuffer->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float2, "a_TexCoord"},
		});
		m_SquareVertexArray->AddVertexBuffer(squarevertexBuffer);


		uint32_t squareindices[] = {
			0, 1, 2, 2, 3, 0
		};

		Hazel::Ref<Hazel::IndexBuffer> squareindexBuffer;
		squareindexBuffer.reset(Hazel::IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareindexBuffer);


		std::string squarevertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position , 1.0);
			}
		)";

		std::string squarefragmentSrc = R"(
			#version 330 core
			
			uniform vec3 u_FlatColor;

			out vec4 FragColor;
			
			void main()
			{
				FragColor = vec4(u_FlatColor, 1.0f);
			}
		)";

		m_SquareShader = Hazel::Shader::Create("Square", squarevertexSrc, squarefragmentSrc);

		std::string texturevertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position , 1.0);
				v_TexCoord = a_TexCoord;
			}
		)";

		std::string texturefragmentSrc = R"(
			#version 330 core
			
			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			out vec4 FragColor;
			
			void main()
			{
				FragColor = texture(u_Texture, v_TexCoord);
			}
		)";

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

		m_Texture = Hazel::Texture2D::Create("assets/textures/1SmwlRi4L6ZTT3WtE4EqT7.jpg");
		m_GoZaRuTexture = Hazel::Texture2D::Create("assets/textures/VTeMZbCLMq2j_9EP-4624AU.png");
	}

	~ExampleLayer()
	{

	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		HZ_CLIENT_TRACE("Timestep: {0}s, {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

		//update
		m_OrthographicCameraController.OnUpdate(ts);

		//Render
		Hazel::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_OrthographicCameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 1.0f));

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->UploadUniformFloat3("u_FlatColor", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 position = glm::vec3(x * 0.16f, y * 0.16f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
				Hazel::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
			}
		}
		
		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVertexArray);
		m_GoZaRuTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVertexArray);

		//Triangle
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		m_OrthographicCameraController.OnEvent(event);
	}

private:
	Hazel::OrthographicCameraController m_OrthographicCameraController;

	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_SquareShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	glm::vec3 m_SquareColor = { 0.2f, 0.1f, 0.6f };

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_GoZaRuTexture;
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