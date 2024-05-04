#include "Sandbox2D.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		:m_Name(name), m_Stopped(false), m_Func(func)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if(!m_Stopped)
			Stop();
	}

	void Stop()
	{
		const auto endTimePoint = std::chrono::high_resolution_clock::now();
		long long duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimePoint - m_StartTimePoint).count();
		
		float ms = duration * 0.001f;
		
		m_Stopped = true;
		m_Func({ m_Name, ms });
	}
private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
	bool m_Stopped;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); });

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthographicCameraController(1280.f/ 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	//update
	{
		PROFILE_SCOPE("m_OrthographicCameraController");
		m_OrthographicCameraController.OnUpdate(ts);
	}

	//render
	{
		PROFILE_SCOPE("Render Prep");
		Hazel::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Render Scene");
		Hazel::Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());

		Hazel::Renderer2D::DrawQuad({ -1.0f, -0.5f }, { 0.5f, 0.5f }, m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);

		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("square color", glm::value_ptr(m_SquareColor));
	
	for (auto profileResult : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms : ");
		strcat(label, profileResult.Name);
		ImGui::Text(label, profileResult.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}
