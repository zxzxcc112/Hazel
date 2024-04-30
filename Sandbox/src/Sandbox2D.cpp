#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthographicCameraController(1280.f/ 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVertexArray = Hazel::VertexArray::Create();

	float squarevertices[] = {
		-0.7f, -0.7f, 0.0f,
		 0.7f, -0.7f, 0.0f,
		 0.7f,  0.7f, 0.0f,
		-0.7f,  0.7f, 0.0f
	};

	Hazel::Ref<Hazel::VertexBuffer> squarevertexBuffer;
	squarevertexBuffer = Hazel::VertexBuffer::Create(squarevertices, sizeof(squarevertices));

	squarevertexBuffer->SetLayout({
		{Hazel::ShaderDataType::Float3, "a_Position"},
		});
	m_SquareVertexArray->AddVertexBuffer(squarevertexBuffer);

	uint32_t squareindices[] = {
			0, 1, 2, 2, 3, 0
	};

	Hazel::Ref<Hazel::IndexBuffer> squareindexBuffer;
	squareindexBuffer = Hazel::IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t));
	m_SquareVertexArray->SetIndexBuffer(squareindexBuffer);

	m_FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	//update
	m_OrthographicCameraController.OnUpdate(ts);

	//render
	Hazel::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_OrthographicCameraController.GetCamera());

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_FlatColor", m_SquareColor);

	Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray);

	Hazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}
