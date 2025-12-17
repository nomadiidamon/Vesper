#include "vzpch.h"
#include "Scene.h"
#include "Vesper/Core/Log.h"
#include "Vesper/Renderer/Renderer2D.h"


namespace Vesper {

	Scene::Scene()
	{
		VZ_PROFILE_FUNCTION();

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		if (m_Registry.try_get<TransformComponent>(entity)) {
			auto& transform = m_Registry.get<TransformComponent>(entity);
		}
	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		VZ_PROFILE_FUNCTION();

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform.Transform, sprite.Color);
		}

	}

}