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
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		friend class Entity;
	};


}