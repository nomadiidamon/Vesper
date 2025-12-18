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
		nameTag.Name = name.empty() ? "Entity" + std::to_string(static_cast<std::uint32_t>(entity)) : name;

		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		VZ_PROFILE_FUNCTION();
		Camera* mainCamera = nullptr;
		glm::mat4* camTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) 
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary) {
					mainCamera = &camera.Camera;
					camTransform = &transform.Transform;
					break;
				}

			}
		}

		if (!mainCamera)
			return;

		Renderer2D::BeginScene(mainCamera->GetProjection(), *camTransform);

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent, TextureAnimationComponent>();
		for (auto entity : group)
		{
			auto& [transform, sprite, texAnim] = group.get<TransformComponent, SpriteRendererComponent, TextureAnimationComponent>(entity);
			texAnim.Update(ts.GetSeconds());
			Renderer2D::DrawQuadWithTexture(transform, texAnim.SubTextures[texAnim.CurrentFrame], 1.0f, sprite.Color);
		}

		Renderer2D::EndScene();

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// resize non fixed aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

}