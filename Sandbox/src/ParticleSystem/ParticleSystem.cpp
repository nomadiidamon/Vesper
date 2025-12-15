#include "ParticleSystem.h"
#include "Vesper/Renderer/Renderer2D.h"
#include "Vesper/Renderer/OrthographicCamera.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>


ParticleSystem::ParticleSystem()
{
	m_PoolIndex = 999;
	m_ParticlePool.resize(1000);
}

ParticleSystem::ParticleSystem(uint32_t maxParticles)
		: m_PoolIndex(maxParticles - 1)
	{
		m_ParticlePool.resize(maxParticles);
	}

	void ParticleSystem::OnUpdate(Vesper::Timestep ts)
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

	void ParticleSystem::OnRender(Vesper::OrthographicCamera& camera)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			Vesper::Renderer2D::DrawQuadRotatedWithTexture(
				{ particle.Position },
				{ size, size },
				Vesper::Renderer2D::GetWhiteTexture(),
				particle.Rotation, 1.0f, color);

		}
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = particleProps.Rotation + particleProps.RotationVariation * (Vesper::Random::Float1() - 0.5f);

		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Vesper::Random::Float1() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Vesper::Random::Float1() - 0.5f);

		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Vesper::Random::Float1() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		particle.LifeTime = particleProps.LifeTime + particleProps.LifetimeVariation * (Vesper::Random::Float1() - 0.5f);
		particle.LifeRemaining = particle.LifeTime;
		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}