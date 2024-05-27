#pragma once
#include <Hazel.h>

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

		//Temp
		Ref<VertexArray> m_SquareVertexArray;
		Ref<Shader> m_FlatColorShader;

		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_Sprite;
		Ref<SubTexture2D> m_SpriteTree;

		uint32_t m_MapWidth = 24;
		uint32_t m_MapHeight = 0;
		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;

		glm::vec4 m_SquareColor = { 0.2f, 0.1f, 0.6f, 1.0f };
	};
}