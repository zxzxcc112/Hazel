#pragma once
#include <Hazel.h>
#include "FlappyBird/ParticleSystem.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& event) override;
private:
	Hazel::OrthographicCameraController m_OrthographicCameraController;

	//Temp
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	Hazel::Ref<Hazel::Framebuffer> m_Framebuffer;

	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;
	Hazel::Ref<Hazel::SubTexture2D> m_Sprite;
	Hazel::Ref<Hazel::SubTexture2D> m_SpriteTree;

	uint32_t m_MapWidth = 24;
	uint32_t m_MapHeight = 0;
	std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> m_TextureMap;

	glm::vec4 m_SquareColor = { 0.2f, 0.1f, 0.6f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_ParticleProps;
};