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
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 160, m_ViewportHeight = 90;
		friend class Entity;
		friend class SceneHierarchyPanel;
	};


}