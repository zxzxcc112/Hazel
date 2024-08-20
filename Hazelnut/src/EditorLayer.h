#pragma once
#include <Hazel.h>

#include "Panels/SceneHierarchyPanel.h"

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
		OrthographicCameraController m_OrthographicCameraController;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Ref<Scene> m_ActiveScene;

		//Temp
		Ref<VertexArray> m_SquareVertexArray;
		Ref<Shader> m_FlatColorShader;
		Ref<Texture2D> m_CheckerboardTexture;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		glm::vec4 m_SquareColor = { 0.2f, 0.1f, 0.6f, 1.0f };

		Entity m_SquareEntity;
		Entity m_PrimaryCameraEntity;
		Entity m_SecondCameraEntity;
		bool m_UsePrimaryCamera = true;

		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}