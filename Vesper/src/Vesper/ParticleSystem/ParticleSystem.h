#pragma once

#include "Vesper.h"

namespace Vesper {

	struct ParticleProps
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };
		glm::vec3 VelocityVariation = { 0.0f, 0.0f, 0.0f };
		glm::vec4 ColorBegin = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4 ColorEnd = { 1.0f, 1.0f, 1.0f, 1.0f };
		float SizeBegin = 1.0f;
		float SizeEnd = 0.0f;
		float SizeVariation = 0.0f;
		float Rotation = 0.0f;
		float RotationVariation = 0.0f;
		float LifeTime = 1.0f;
		float LifetimeVariation = 0.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();
		ParticleSystem(uint32_t maxParticles);

		void OnUpdate(Vesper::Timestep ts);
		void OnRender(Vesper::OrthographicCamera& camera);
		void Emit(const ParticleProps& particleProps);
		void SetParticleProps(const ParticleProps& particleProps) { m_Props = particleProps; }

	private:
		struct Particle
		{
			glm::vec3 Position;
			glm::vec3 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float SizeBegin, SizeEnd;
			float Rotation;
			float LifeTime = 0.0f;
			float LifeRemaining = 0.0f;
			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;
		ParticleProps m_Props;
	};


}