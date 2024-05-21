#include "ParticleSystem.h"
#include "Random.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	:m_MaxParticles(maxParticles), m_PoolIndex(maxParticles - 1)
{
	m_ParticlePool.resize(maxParticles);
}

void ParticleSystem::Emit(const ParticleProps& props)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = props.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = props.Velocity;
	particle.Velocity.x += props.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += props.VelocityVariation.y * (Random::Float() - 0.5f);

	// Life
	particle.LifeTime = props.LifeTime;
	particle.LifeRemaining = props.LifeTime;

	// Color
	particle.ColorBegin = props.ColorBegin;
	particle.ColorEnd = props.ColorEnd;

	// Size
	particle.SizeBegin = props.SizeBegin + props.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = props.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

void ParticleSystem::OnUpdate(Hazel::Timestep ts)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRneder()
{
	for (const auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
		Hazel::Renderer2D::DrawRotatedQuad(particle.Position, { size, size }, particle.Rotation, color);
	}
}
