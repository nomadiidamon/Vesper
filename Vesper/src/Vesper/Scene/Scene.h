#pragma once
#include <entt.hpp>
#include "Components.h"
#include "Vesper/Core/Timestep.h"


namespace Vesper {

	class Entity;
	class EditorCamera;

	class Scene
	{
	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		// Temp-> add entity wrapper later
		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(const std::string& name, const std::string& uuid);
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		std::string m_Name;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 160, m_ViewportHeight = 90;
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		/// TODO: friend class SceneCamera;
		/// TODO: friend class SceneRenderer;

		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }
	};


}