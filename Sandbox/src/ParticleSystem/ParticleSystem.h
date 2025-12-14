#pragma once

#include "Vesper.h"


	struct ParticleProps
	{
		glm::vec3 Position;
		glm::vec3 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float Rotation, RotationVariation;
		float LifeTime = 0, LifetimeVariation = 0;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();
		ParticleSystem(uint32_t maxParticles);

		void OnUpdate(Vesper::Timestep ts);
		void OnRender(Vesper::OrthographicCamera& camera);

		void Emit(const ParticleProps& particleProps);

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
	};

