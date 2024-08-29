#pragma once

#include "Hazel.h"
#include "Player.h"

struct Pillar
{
	glm::vec3 TopPos = { 0.0f, 15.0f, -0.1f };
	glm::vec2 TopScale = { 10.0f, 30.0f};

	glm::vec3 BtmPos = { 0.0f, -15.0f, -0.1f };
	glm::vec2 BtmScale = { 10.0f, 30.0f };
};

class Level
{
public:
	void Init();

	void OnUpdate(Hazel::Timestep ts);
	void OnRender();
	void OnImGuiRender();

	void Reset();

	const bool IsGameOver() const { return m_IsGameOver; }
	const Player& GetPlayer() const { return m_Player; }
private:
	void CreatePillar(int index, float offset);
	bool CollisionTest();
	void GameOver();
private:
	Player m_Player;
	glm::vec4 m_PlayerTransformedVerts[4];

	std::vector<Pillar> m_Pillars;
	int m_PillarTarget = 0;
	glm::vec3 m_PillarHSV = { 0.0f, 0.8f, 0.8f };

	bool m_IsGameOver = false;

	float m_FCScale = 33.0f;
	Hazel::Ref<Hazel::Texture2D> m_TriangleTexture;
};