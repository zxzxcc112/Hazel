#include "Sandbox2D.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

static uint32_t s_MapWidth = 24;
static const char* s_MapTiles = 
"WWWWWWWWWWWWWWWWWWWWWWWW"
"DWWWWWWWWWWNWWWWWWWWWWWW"
"WWWWWDDDDWWWWWWDDDDWWWWW"
"DWWWDDDWDDWWWWDDDDDDWWWW"
"WWWWDDDWWWDDWWDDDDDDWWWW"
"DWWWWWDWWWWDWWDDDDDDWWWW"
"WWWWWWWDDDDWWWWDDDDWWWWW"
"DWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthographicCameraController(1280.f/ 720.0f, true), m_ParticleSystem(1000)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	m_Sprite = Hazel::SubTexture2D::CreateFormCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 });
	m_SpriteTree = Hazel::SubTexture2D::CreateFormCoords(m_SpriteSheet, { 0, 1 }, { 128, 128 }, { 1, 2 });

	m_TextureMap['W'] = Hazel::SubTexture2D::CreateFormCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });
	m_TextureMap['D'] = Hazel::SubTexture2D::CreateFormCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = (uint32_t)strlen(s_MapTiles) / m_MapWidth;

	m_OrthographicCameraController.SetZoomLevel(5.0f);

	// Init here
	m_ParticleProps.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_ParticleProps.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_ParticleProps.SizeBegin = 0.3f, m_ParticleProps.SizeVariation = 0.1f, m_ParticleProps.SizeEnd = 0.0f;
	m_ParticleProps.LifeTime = 3.0f;
	m_ParticleProps.Velocity = { 0.0f, 0.0f };
	m_ParticleProps.VelocityVariation = { 3.0f, 1.0f };
	m_ParticleProps.Position = { 0.0f, 0.0f };
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

	if (Hazel::Input::IsMouseButtonPressed(Hazel::MouseCode::Button0))
	{
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();
		auto [mouseX, mouseY] = Hazel::Input::GetMousePosition();
		auto aspectRatio = m_OrthographicCameraController.GetAspectRatio();
		auto zoomLevel = m_OrthographicCameraController.GetZoomLevel();
		auto worldXDelta = (mouseX / width) * (2.0f * aspectRatio * zoomLevel) - (2 * aspectRatio * zoomLevel) * 0.5f;
		auto worldYDelta = -((mouseY / height) * (2.0f * zoomLevel) - (2.0f * zoomLevel) * 0.5f);
		glm::vec3 world = m_OrthographicCameraController.GetCamera().GetPosition();
		float worldRotation = m_OrthographicCameraController.GetCamera().GetRotation();
		float rx = world.x + worldXDelta * cos(glm::radians(worldRotation)) - worldYDelta * sin(glm::radians(worldRotation));
		float ry = world.y + worldXDelta * sin(glm::radians(worldRotation)) + worldYDelta * cos(glm::radians(worldRotation));
		//HZ_CLIENT_TRACE("world pos: {0}, {1}", rx, ry);
		m_ParticleProps.Position = {rx, ry};
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_ParticleProps);
	}

	m_ParticleSystem.OnUpdate(ts);

	//render
	Hazel::Renderer2D::ResetStatistics();
	{
		HZ_PROFILE_SCOPE("Rendering Prep");
		Hazel::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += 90.0f * ts;

		HZ_PROFILE_SCOPE("Rendering Scene");
		Hazel::Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());
		/*Hazel::Renderer2D::DrawRotatedQuad({ -1.0f, -0.5f }, { 0.5f, 0.5f }, glm::radians(45.0f), m_SquareColor);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, -3.0f }, { 0.5f, 0.5f }, glm::radians(rotation), m_SquareColor);
		Hazel::Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.095f}, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 20.0f);
		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());
		for (float x = -5.0f; x < 5.0f; x+=0.5f)
		{
			for (float y = -5.0f; y < 5.0f; y+=0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.8f };
				Hazel::Renderer2D::DrawQuad({x, y, -0.09f}, { 0.45f, 0.45f }, color);
			}
		}*/
		m_ParticleSystem.OnRneder();

		//Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, m_SpriteSheet);
		//Hazel::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, m_Sprite);
		//Hazel::Renderer2D::DrawQuad({ 2.0f, 0.0f, 0.1f }, { 1.0f, 2.0f }, m_SpriteTree);


		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				Hazel::Ref<Hazel::SubTexture2D> texture = m_Sprite;
				if (m_TextureMap.find(tileType) != m_TextureMap.end())
					texture = m_TextureMap[tileType];

				Hazel::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.1f }, { 1.0f, 1.0f }, texture);
			}
		}

		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	static bool dockspaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
			if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
			if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false))
				Hazel::Application::Get().Close();
			ImGui::EndMenu();
		}
		

		ImGui::EndMenuBar();
	}

	ImGui::End();

	ImGui::Begin("Settings");
	ImGui::Text("Statistics:");
	ImGui::Text("Draw Call: %d", Hazel::Renderer2D::GetStatistics().DrawCalls);
	ImGui::Text("Quads: %d", Hazel::Renderer2D::GetStatistics().QuadCount);
	ImGui::Text("Vertices: %d", Hazel::Renderer2D::GetStatistics().GetVertexCount());
	ImGui::Text("Indices: %d", Hazel::Renderer2D::GetStatistics().GetIndexCount());

	ImGui::ColorEdit4("square color", glm::value_ptr(m_SquareColor));
	ImGui::Image((void*)(size_t)m_CheckerboardTexture->GetRendererID(), {256.0f, 256.0f});
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}