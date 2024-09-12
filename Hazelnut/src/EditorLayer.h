#pragma once
#include <Hazel.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Hazel/Renderer/EditorCamera.h"

namespace Hazel
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
    private:
        bool OnKeyPressed(KeyPressedEvent& e);

        void OpenScene();
        void SaveSceneAs();
        void NewScene();
	private:
		OrthographicCameraController m_OrthographicCameraController;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Ref<Scene> m_ActiveScene;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		SceneHierarchyPanel m_SceneHierarchyPanel;
        EditorCamera m_EditorCamera;
        int m_GizmoOperation = -1;

		//Temp
		Ref<VertexArray> m_SquareVertexArray;
		Ref<Shader> m_FlatColorShader;
		Ref<Texture2D> m_CheckerboardTexture;

		glm::vec4 m_SquareColor = { 0.2f, 0.1f, 0.6f, 1.0f };

		Entity m_SquareEntityA;
		Entity m_SquareEntityB;
		Entity m_PrimaryCameraEntity;
		Entity m_SecondCameraEntity;
		bool m_UsePrimaryCamera = true;
	};
}