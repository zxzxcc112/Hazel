#pragma once
#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer();
	~ExampleLayer();

	void OnUpdate(Hazel::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& event) override;

private:
	Hazel::OrthographicCameraController m_OrthographicCameraController;

	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_SquareShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	glm::vec3 m_SquareColor = { 0.2f, 0.1f, 0.6f };

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_GoZaRuTexture;
};