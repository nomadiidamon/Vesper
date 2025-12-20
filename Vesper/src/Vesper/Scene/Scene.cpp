#include "vzpch.h"
#include "Scene.h"
#include "Vesper/Core/Log.h"
#include "Vesper/Renderer/Renderer2D.h"
#include "Vesper/Scene/Entity.h"
#include "Vesper/Scene/ScriptableEntity.h"

namespace Vesper {

	Scene::Scene()
	{
		VZ_PROFILE_FUNCTION();


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

		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
			{
				/// TODO: Move to Scene::OnScenePlay()
				if (!nsc.Instance) 
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);

			});

		}

		Camera* mainCamera = nullptr;
		glm::mat4* camTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary) {
					mainCamera = &camera.Camera;
					camTransform = &transform.GetTransform();
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
			auto [transform, sprite, texAnim] = group.get<TransformComponent, SpriteRendererComponent, TextureAnimationComponent>(entity);
			texAnim.Update(ts.GetSeconds());
			Renderer2D::DrawQuadWithTexture(transform.GetTransform(), texAnim.SubTextures[texAnim.CurrentFrame], 1.0f, sprite.Color);
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