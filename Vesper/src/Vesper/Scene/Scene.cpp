#include "vzpch.h"
#include "Scene.h"
#include "Vesper/Core/Log.h"
#include "Vesper/Renderer/Renderer2D.h"
#include "Vesper/Scene/Entity.h"

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

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& nameTag = entity.AddComponent<NameComponent>();
		nameTag.Name = name.empty() ? "Entity" + std::to_string(static_cast<std::uint32_t>(entity) ) : name;

		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		VZ_PROFILE_FUNCTION();

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent, TextureAnimationComponent>();
		for (auto entity : group)
		{
			auto& [transform, sprite, texAnim] = group.get<TransformComponent, SpriteRendererComponent, TextureAnimationComponent>(entity);
			texAnim.Update(ts.GetSeconds());
			Renderer2D::DrawQuadWithTexture(transform, texAnim.SubTextures[texAnim.CurrentFrame], 1.0f, sprite.Color);
		}

	}

}