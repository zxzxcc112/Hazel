#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Scene/SceneSerializer.h"
#include "Hazel/Utils/PlatformUtils.h"

#include <ImGuizmo.h>

#include "Hazel/Math/Math.h"

namespace Hazel
{
	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_OrthographicCameraController(1280.f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Noto_Sans_TC/static/NotoSansTC-Regular.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF("assets/fonts/Noto_Sans_TC/static/NotoSansTC-Bold.ttf", 18.0f);

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

        FramebufferSpecification spec;
        spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH };
        spec.Width = 1280;
        spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntityA = m_ActiveScene->CreateEntity("Square Entity A");
		m_SquareEntityA.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 0.8f, 0.0f, 1.0f });

		m_SquareEntityB = m_ActiveScene->CreateEntity("Square Entity B");
		m_SquareEntityB.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.0f, 0.0f, 1.0f });
		m_SquareEntityB.GetComponent<TransformComponent>().Translation.x += 0.5f;

		m_PrimaryCameraEntity = m_ActiveScene->CreateEntity("Primary Camera Entity");
		m_PrimaryCameraEntity.AddComponent<CameraComponent>();

		m_SecondCameraEntity = m_ActiveScene->CreateEntity("Second Camera Entity");
		m_SecondCameraEntity.AddComponent<CameraComponent>();
		m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				HZ_CORE_WARN("CameraController is created.");
			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(KeyCode::W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(KeyCode::S))
					translation.y -= speed * ts;
				if (Input::IsKeyPressed(KeyCode::A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(KeyCode::D))
					translation.x += speed * ts;
			}

			void OnDestroy()
			{

			}
		};

		m_PrimaryCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();

		//resize frame buffer
		const FramebufferSpecification& spec = m_Framebuffer->GetSpecification();
        if (m_ViewportSize.x != 0 && m_ViewportSize.y != 0 && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_OrthographicCameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		//update
		{
			HZ_PROFILE_SCOPE("m_OrthographicCameraController.OnUpdate");
            if (m_ViewportFocused)
                m_OrthographicCameraController.OnUpdate(ts);

            m_EditorCamera.OnUpdate(ts);
		}

		//render
		Renderer2D::ResetStatistics();
		{
			HZ_PROFILE_SCOPE("Rendering Prep");
			m_Framebuffer->Bind();

			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();
		}

		{
			static float rotation = 0.0f;
			rotation += 90.0f * ts;

			HZ_PROFILE_SCOPE("Rendering Scene");
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

			Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());
			//Renderer2D::DrawRotatedQuad({ -1.0f, -0.5f }, { 0.5f, 0.5f }, glm::radians(45.0f), m_SquareColor);
			//Renderer2D::DrawRotatedQuad({ 0.0f, -3.0f }, { 0.5f, 0.5f }, glm::radians(rotation), m_SquareColor);
			//Renderer2D::DrawQuad({ 1.0f, 1.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
			//Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
			//Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.08f}, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 1.0f);
			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());
			//for (float x = -5.0f; x < 5.0f; x+=0.5f)
			//{
			//	for (float y = -5.0f; y < 5.0f; y+=0.5f)
			//	{
			//		glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f, 0.8f };
			//		Renderer2D::DrawQuad({x, y, -0.09f}, { 0.45f, 0.45f }, color);
			//	}
			//}
			Renderer2D::EndScene();
			m_Framebuffer->UnBind();
		}
	}

	void EditorLayer::OnImGuiRender()
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
		ImGuiStyle& style = ImGui::GetStyle();
		float windowMinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = windowMinSizeX;


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
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    OpenScene();
                }
                
                if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
                {
                    SaveSceneAs();
                }
				ImGui::Separator();
				if (ImGui::MenuItem("Close", NULL, false))
					Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
		ImGui::ShowDemoWindow();
		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		ImGui::Text("Render 2D stats:");
		ImGui::Text("Draw Call: %d", Renderer2D::GetStatistics().DrawCalls);
		ImGui::Text("Quads: %d", Renderer2D::GetStatistics().QuadCount);
		ImGui::Text("Vertices: %d", Renderer2D::GetStatistics().GetVertexCount());
		ImGui::Text("Indices: %d", Renderer2D::GetStatistics().GetIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		ImGui::Image((void*)(size_t)m_Framebuffer->GetColorAttachmentID(), viewportPanelSize, { 0, 1 }, { 1, 0 });

        // Gizmos
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

        if (selectedEntity && m_GizmoOperation != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // Camera
            
            // Runtime camera from entity
            // Entity primaryCamera = m_ActiveScene->GetPrimaryCameraEntity();
            // const auto& cameraTransformComponent = primaryCamera.GetComponent<TransformComponent>();
            // const auto cameraView = glm::inverse(cameraTransformComponent.GetTransform());
            // 
            // const auto& cameraComponent = primaryCamera.GetComponent<CameraComponent>();
            // const auto& camera = cameraComponent.Camera;
            // const auto& cameraProjection = camera.GetProjection();
            // 
            // ImGuizmo::SetOrthographic(camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic ? true : false);

            // Editor Camera
            const auto& cameraView = m_EditorCamera.GetViewMatrix();
            const auto& cameraProjection = m_EditorCamera.GetProjection();

            // Selected Entity
            auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
            auto matrix = transformComponent.GetTransform();

            bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale
            // Snap to 45 degrees for rotation;
            if (m_GizmoOperation == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoOperation,
                ImGuizmo::MODE::LOCAL, glm::value_ptr(matrix), nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(matrix, translation, rotation, scale);

                auto deltaRotation = rotation - transformComponent.Rotation;
                transformComponent.Translation = translation;
                transformComponent.Rotation += deltaRotation;
                transformComponent.Scale = scale;
            }
        }

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_OrthographicCameraController.OnEvent(e);
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GeteRepeatCount() > 0)
            return false;
        
        bool ctrlPressed = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
        bool shiftPressed = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

        switch (e.GetKeyCode())
        {
            case KeyCode::N:
            {
                if (ctrlPressed)
                    NewScene();
                break;
            }
            case KeyCode::O:
            {
                if(ctrlPressed)
                    OpenScene();
                break;
            }
            case KeyCode::S:
            {
                if (ctrlPressed && shiftPressed)
                    SaveSceneAs();
                break;
            }
            // Gizmo
            case KeyCode::Q:
            {
                m_GizmoOperation = -1;
                break;
            }
            case KeyCode::W:
            {
                m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case KeyCode::E:
            {
                m_GizmoOperation = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case KeyCode::R:
            {
                m_GizmoOperation = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }
        return false;
    }

    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Hazel File(.hazel)\0*.hazel\0All File\0*.*\0");
        if (!filepath.empty())
        {
            NewScene();

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Hazel File(.hazel)\0*.hazel\0All File\0*.*\0");
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
}