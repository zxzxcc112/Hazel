#include "GameLayer.h"
#include "Random.h"

GameLayer::GameLayer()
	:Layer("Flappy Bird")
{
	auto& window = Hazel::Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());

	Random::Init();
}

void GameLayer::OnAttach()
{
	m_Level.Init();

	ImGuiIO io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", 120.0f);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Hazel::Timestep ts)
{
	m_Time += ts;
	if ((int)(m_Time * 10.0f) % 8 > 4)
		m_Blink = !m_Blink;

	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;

	const auto& PlayerPos = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ PlayerPos, 0.0f });

	if (m_State == GameState::Play)
		m_Level.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	Hazel::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	//m_Level.OnImGuiRender();

	switch (m_State)
	{
		case GameState::Play:
		{
			uint32_t playerScore = m_Level.GetPlayer().GetScore();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
			break;
		}
		case GameState::MainMenu:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = Hazel::Application::Get().GetWindow().GetWidth();
			auto height = Hazel::Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 50.0f;
			if (m_Blink)
				ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
			break;
		}
		case GameState::GameOver:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = Hazel::Application::Get().GetWindow().GetWidth();
			auto height = Hazel::Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 50.0f;
			if (m_Blink)
				ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

			pos.x += 200.0f;
			pos.y += 150.0f;
			uint32_t playerScore = m_Level.GetPlayer().GetScore();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
			break;
		}
	}
}

void GameLayer::OnEvent(Hazel::Event& e)
{
	Hazel::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(GameLayer::OnWindowResized));
	dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(GameLayer::OnKeyPressed));
}

bool GameLayer::OnWindowResized(Hazel::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}

bool GameLayer::OnKeyPressed(Hazel::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == HZ_KEY_P)
		m_Level.Reset();

	m_State = GameState::Play;
	return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / height;

	float camHeight = 15.0f;
	float top = camHeight;
	float bottom = -camHeight;
	float left = -camHeight * aspectRatio;
	float right = camHeight * aspectRatio;
	m_Camera = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
}
