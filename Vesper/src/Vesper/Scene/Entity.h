#pragma once

#include "entt.hpp"

#include "Scene.h"

namespace Vesper {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent() const 
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityID);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			VZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			return m_Scene->m_Registry.emplace_or_replace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() 
		{
			VZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityID);
		}

		template<typename T, typename... Args>
		T& GetOrAddComponent(Args&&... args)
		{
			if (HasComponent<T>())
				return GetComponent<T>();
			else
				return AddComponent<T>(std::forward<Args>(args)...);
		}
	
		template<typename T>
		void RemoveComponent() 
		{
			VZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityID);
		}

		operator bool() const { return m_EntityID != entt::null; }
		operator entt::entity() const { return m_EntityID; }
		operator uint32_t() const { return (uint32_t)m_EntityID; }
		bool operator==(const Entity& other) const
		{
			return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene;
		}
		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_EntityID {entt::null};
		Scene* m_Scene = nullptr;
	};


}