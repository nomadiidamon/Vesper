#pragma once
#include <entt.hpp>
#include "Components.h"
#include "Vesper/Core/Timestep.h"

namespace Vesper {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		// Temp-> add entity wrapper later
		Entity CreateEntity(const std::string& name = std::string());
		
		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
		friend class Entity;
	};


}