#include "Sandbox2D.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthographicCameraController(1280.f/ 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Texture2 = Hazel::Texture2D::Create("assets/textures/160.png");
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	//update
	{
		HZ_PROFILE_SCOPE("m_OrthographicCameraController.OnUpdate");
		m_OrthographicCameraController.OnUpdate(ts);
	}

	//render
	Hazel::Renderer2D::ResetStatistics();
	{
		HZ_PROFILE_SCOPE("Rendering Prep");
		Hazel::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += 90.0f * ts;

		HZ_PROFILE_SCOPE("Rendering Scene");
		Hazel::Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());
		Hazel::Renderer2D::DrawRotatedQuad({ -1.0f, -0.5f }, { 0.5f, 0.5f }, 45.0f, m_SquareColor);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, -3.0f }, { 0.5f, 0.5f }, rotation, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f}, { 1.0f, 1.0f }, rotation, m_Texture2, 20.0f);
		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());
		for (float x = -5.0f; x < 5.0f; x+=0.1f)
		{
			for (float y = -5.0f; y < 5.0f; y+=0.1f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.8f };
				Hazel::Renderer2D::DrawQuad({x, y, -0.09f}, { 0.45f, 0.45f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::Text("Statistics:");
	ImGui::Text("Draw Call: %d", Hazel::Renderer2D::GetStatistics().DrawCalls);
	ImGui::Text("Quads: %d", Hazel::Renderer2D::GetStatistics().QuadCount);
	ImGui::Text("Vertices: %d", Hazel::Renderer2D::GetStatistics().GetVertexCount());
	ImGui::Text("Indices: %d", Hazel::Renderer2D::GetStatistics().GetIndexCount());

	ImGui::ColorEdit4("square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}
