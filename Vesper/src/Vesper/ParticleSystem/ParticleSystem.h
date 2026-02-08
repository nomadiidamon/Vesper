#pragma once
/// @file ParticleSystem.h
/// @author Damon S. Green II
/// @brief Defines the ParticleSystem class, which is responsible for rendering a simple particle system. This is a temporary implementation and will be replaced with a more robust implementation in the future.


#include "Vesper.h"
#include "Vesper/Renderer/OrthographicCamera.h"

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


	/// @class ParticleSystem
	/// @brief A simple particle system for stress testing the renderer. This is a temporary starter particle system and will be replaced with a more robust implementation in the future.
	/// @note Temporary implementation is not optimized for performance and is intended for testing purposes only. 
	/// It uses a simple pool of particles and updates them each frame. The particle properties can be set using the ParticleProps struct, which allows for variation in velocity, size, rotation, and lifetime.
	class ParticleSystem
	{
	public:
		ParticleSystem();
		ParticleSystem(uint32_t maxParticles);

		void OnUpdate(Timestep ts);
		void OnRender(OrthographicCamera& camera);
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

	public:
		float m_TimeSinceLastEmit = 0.0f;
		bool m_IsEmitting = true;
	};


}