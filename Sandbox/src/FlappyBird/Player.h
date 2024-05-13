#pragma once
#include "Hazel.h"
#include "ParticleSystem.h"

class Player
{
public:
	Player();
	~Player() = default;

	void OnUpdate(Hazel::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	glm::vec2 GetPosition() const { return m_Position; }
	float GetRotation() const { return m_Velocity.y * 4.0f - 90.0f; }

	void LoadAssets();
	void Reset();

	uint32_t GetScore() const { return (uint32_t)m_Position.x; }
private:
	glm::vec2 m_Position = { 0.0f, 0.0f };
	glm::vec2 m_Velocity = { 6.0f, 0.0f };

	float m_EnginePower = 2.0f;
	float m_Gravity = 0.5f;

	float m_Time = 0.0f;
	float m_NextEmitInternal = 0.2f;
	float m_NextEmitTime = m_NextEmitInternal;

	ParticleProps m_SmokeParticleProps, m_EngineParticleProps;
	ParticleSystem m_ParticleSystem;

	Hazel::Ref<Hazel::Texture2D> m_PlayerTexture;
};