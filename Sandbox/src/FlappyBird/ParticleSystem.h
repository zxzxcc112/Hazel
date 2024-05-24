#pragma once
#include <Hazel.h>

struct ParticleProps
{
	glm::vec2 Position{ 0.0f };
	glm::vec2 Velocity{ 0.0f }, VelocityVariation{ 0.0f };
	glm::vec4 ColorBegin{ 0.0f }, ColorEnd{ 0.0f };
	float SizeBegin{ 0.0f }, SizeEnd{ 0.0f }, SizeVariation{ 0.0f };
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem(uint32_t maxParticles);

	void Emit(const ParticleProps& props);

	void OnUpdate(Hazel::Timestep ts);
	void OnRneder();
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = true;
	};
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex;
	uint32_t m_MaxParticles;
};