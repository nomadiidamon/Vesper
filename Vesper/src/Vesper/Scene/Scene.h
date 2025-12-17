#pragma once
#include <entt.hpp>
#include "Components.h"
#include "Vesper/Core/Timestep.h"

namespace Vesper {

	class Scene
	{
	public:
		Scene();
		~Scene();

		// Temp-> add entity wrapper later
		entt::entity CreateEntity();

		// Temp-> add registry access wrapper later
		entt::registry& Reg() { return m_Registry; }
		
		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
	};


}