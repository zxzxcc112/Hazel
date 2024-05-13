#include "Player.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

Player::Player()
{
	m_SmokeParticleProps.Position = { 0.0f, 0.0f };
	m_SmokeParticleProps.Velocity = { -2.0f, 0.0f }, m_SmokeParticleProps.VelocityVariation = { 4.0f, 2.0f };
	m_SmokeParticleProps.LifeTime = 4.0f;
	m_SmokeParticleProps.ColorBegin = { 0.8f, 0.8f, 0.8f, 1.0f };
	m_SmokeParticleProps.ColorEnd = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_SmokeParticleProps.SizeBegin = 0.35f, m_SmokeParticleProps.SizeEnd = 0.0f, m_SmokeParticleProps.SizeVariation = 0.15f;

	m_EngineParticleProps.Position = { 0.0f, 0.0f };
	m_EngineParticleProps.Velocity = { -2.0f, 0.0f }, m_EngineParticleProps.VelocityVariation = { 4.0f, 2.0f };
	m_EngineParticleProps.LifeTime = 1.0f;
	m_EngineParticleProps.ColorBegin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_EngineParticleProps.ColorEnd = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f , 1.0f };
	m_EngineParticleProps.SizeBegin = 0.5f, m_EngineParticleProps.SizeEnd = 0.0f, m_EngineParticleProps.SizeVariation = 0.3f;
}

void Player::OnUpdate(Hazel::Timestep ts)
{
	m_Time += ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_SPACE))
	{
		m_Velocity.y += m_EnginePower;

		// Flames
		glm::vec2 emissionPoint = { 0.0f, -0.6f };
		float rotation = glm::radians(GetRotation());
		glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::vec4(emissionPoint, 0.0f, 1.0f);
		m_EngineParticleProps.Position = m_Position + glm::vec2(rotated.x, rotated.y);
		m_EngineParticleProps.Velocity.y = -m_Velocity.y * 0.2f - 0.2f;
		m_ParticleSystem.Emit(m_EngineParticleProps);
	}
	else
	{
		m_Velocity.y -= m_Gravity;
	}

	m_Velocity.y = glm::clamp(m_Velocity.y, -20.0f, 20.0f);
	m_Position += m_Velocity * (float)ts;

	if (m_Time > m_NextEmitTime)
	{
		m_SmokeParticleProps.Position = m_Position;
		m_ParticleSystem.Emit(m_SmokeParticleProps);
		m_NextEmitTime += m_NextEmitInternal;
	}

	m_ParticleSystem.OnUpdate(ts);
}

void Player::OnRender()
{
	Hazel::Renderer2D::DrawRotatedQuad(m_Position, { 1.0f, 1.5f }, glm::radians(GetRotation()), m_PlayerTexture);

	m_ParticleSystem.OnRneder();
}

void Player::OnImGuiRender()
{
	ImGui::Begin("Player Setting");
	ImGui::DragFloat("Engine Power", &m_EnginePower, 0.1f, 0.0f, 5.0f);
	ImGui::DragFloat("Gravity", &m_Gravity, 0.1f, 0.0f, 5.0f);
	ImGui::DragFloat("Velocity", &m_Velocity.x, 0.1f, 0.0f, 20.0f);
	ImGui::Text("pos: %.2f, %.2f", m_Position.x, m_Position.y);
	ImGui::End();
}

void Player::LoadAssets()
{
	m_PlayerTexture = Hazel::Texture2D::Create("assets/textures/ship.png");
}

void Player::Reset()
{
	m_Position = { 0.0f, 0.0f };
	m_Velocity = { 6.0f, 0.0f };
}
