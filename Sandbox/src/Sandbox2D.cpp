#include "Sandbox2D.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthographicCameraController(1280.f/ 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	
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

	Hazel::Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());

	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, m_SquareColor);
	Hazel::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	Hazel::Renderer2D::EndScene();
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
