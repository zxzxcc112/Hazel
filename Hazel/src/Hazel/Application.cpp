#include "hzpch.h"
#include "Application.h"

#include <glad/glad.h>
#include "Hazel/Renderer/Renderer.h"

#include "Input.h"
#include "KeyCodes.h"

namespace Hazel
{
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	

	Application::Application()
		:m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);



		m_VertexArray.reset(VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.2f, 0.7f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.7f, 0.2f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.1f, 0.7f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		

		uint32_t indices[] = {
			0, 1, 2,
		};

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		
		//square
		m_SquareVertexArray.reset(VertexArray::Create());

		float squarevertices[] = {
			-0.7f, -0.7f, 0.0f,
			 0.7f, -0.7f, 0.0f,
			 0.7f,  0.7f, 0.0f,
			-0.7f,  0.7f, 0.0f,
		};

		std::shared_ptr<VertexBuffer> squarevertexBuffer;
		squarevertexBuffer.reset(VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

		squarevertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
		});
		m_SquareVertexArray->AddVertexBuffer(squarevertexBuffer);


		uint32_t squareindices[] = {
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<IndexBuffer> squareindexBuffer;
		squareindexBuffer.reset(IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t)));
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

		m_SquareShader.reset(new Shader(squarevertexSrc, squarefragmentSrc));
	}

	Application::~Application() {}

	void Application::Run()
	{
		HZ_CORE_INFO("The Hazel Application is running.");

		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_SquareShader, m_SquareVertexArray);
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_Layers)
				layer->OnUpdate();

			// TODO: run on rendering thread
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_Layers)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnCameraTransform));

		HZ_CORE_TRACE("{0}", event);

		for (auto it = m_Layers.end(); it != m_Layers.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_Layers.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_Layers.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnCameraTransform(KeyPressedEvent& event)
	{
		glm::vec3 position = m_Camera.GetPosition();
		float rotation = m_Camera.GetRotation();

		switch (event.GetKeyCode())
		{
			case HZ_KEY_W: m_Camera.SetPosition(position + glm::vec3(  0.0f,  0.1f, 0.0f)); break;
			case HZ_KEY_A: m_Camera.SetPosition(position + glm::vec3( -0.1f,  0.0f, 0.0f)); break;
			case HZ_KEY_S: m_Camera.SetPosition(position + glm::vec3(  0.0f, -0.1f, 0.0f)); break;
			case HZ_KEY_D: m_Camera.SetPosition(position + glm::vec3(  0.1f,  0.0f, 0.0f)); break;
			case HZ_KEY_Q: m_Camera.SetRotation(rotation + -1.0f); break;
			case HZ_KEY_E: m_Camera.SetRotation(rotation +  1.0f); break;
		}

		return false;
	}
}