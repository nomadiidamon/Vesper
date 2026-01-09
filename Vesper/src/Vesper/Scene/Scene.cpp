#include "vzpch.h"
#include "Scene.h"
#include "Vesper/Core/Log.h"
#include "Vesper/Renderer/Renderer2D.h"
#include "Vesper/Scene/Entity.h"
#include "Vesper/Scene/ScriptableEntity.h"

namespace Vesper {

	Scene::Scene()
		: m_Name("Untitled Scene")
	{
		VZ_PROFILE_FUNCTION();

	}

	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
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
		entity.AddComponent<UUIDComponent>();

		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name, const std::string& uuid)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& nameTag = entity.AddComponent<NameComponent>();
		nameTag.Name = name.empty() ? "Entity" + std::to_string(static_cast<std::uint32_t>(entity)) : name;
		entity.AddComponent<UUIDComponent>(uuid);
		return entity;

	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
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


		auto view = m_Registry.group<SpriteRendererComponent>();
		for (auto entity : view)
		{
			auto& transform = m_Registry.get<TransformComponent>(entity);
			auto& sprite = m_Registry.get<SpriteRendererComponent>(entity);

			// Do not render subtextures here
			auto stc = m_Registry.try_get<SubTextureComponent>(entity);
			if (stc) {
				continue;
			}

			if (sprite.TextureEnabled && !sprite.Texture)
				Renderer2D::DrawQuadWithTexture(transform.GetTransform(), VZ_DEFAULT_TEXTURE, sprite.TilingFactor, sprite.Color);
			else if (sprite.TextureEnabled && sprite.Texture)
				Renderer2D::DrawQuadWithTexture(transform.GetTransform(), sprite.Texture, sprite.TilingFactor, sprite.Color);
			else
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
		}

		auto subTextureView = m_Registry.group<SubTextureComponent>();
		for (auto entity : subTextureView) {
			auto& transform = m_Registry.get<TransformComponent>(entity);
			auto& sprite = m_Registry.get<SpriteRendererComponent>(entity);
			auto& subTexture = m_Registry.get<SubTextureComponent>(entity);
			

			if (!sprite.TextureEnabled) sprite.TextureEnabled = true;
			if (subTexture.SubTexture == nullptr)
			{
				subTexture.SetTexture(VZ_DEFAULT_TEXTURE);
			}
			else {
				// Ensure the subtexture's texture matches the sprite's texture
				if (sprite.Texture && subTexture.SubTexture->GetTexture() != sprite.Texture) {
					subTexture.SetTexture(sprite.Texture);
				}
			}

			Renderer2D::DrawQuadWithTexture(transform.GetTransform(), subTexture.GetSubTexture(), sprite.TilingFactor, sprite.Color);
		}

		//auto group1 = m_Registry.group<TextureAnimationComponent>();
		//for (auto entity : group1)
		//{
		//	auto& transform = m_Registry.get<TransformComponent>(entity);
		//	auto& texAnim = m_Registry.get<TextureAnimationComponent>(entity);
		//	auto& sprite = m_Registry.get<SpriteRendererComponent>(entity);
		//	texAnim.Update(ts.GetSeconds());
		//	Renderer2D::DrawQuadWithTexture(transform.GetTransform(), texAnim.SubTextures[texAnim.CurrentFrame], 1.0f, sprite.Color);
		//}

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

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<UUIDComponent>(Entity entity, UUIDComponent& component) {
		// TODO: search registry to ensure unique UUID
	}

	template<>
	void Scene::OnComponentAdded<NameComponent>(Entity entity, NameComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded<SubTextureComponent>(Entity entity, SubTextureComponent& component) {
		auto& src = entity.GetOrAddComponent<SpriteRendererComponent>();
		if (!src.TextureEnabled) src.TextureEnabled = true;
		component.SetTexture(src.Texture ? src.Texture : VZ_DEFAULT_TEXTURE);
	}

	template<>
	void Scene::OnComponentAdded<TextureAnimationComponent>(Entity entity, TextureAnimationComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {
	}

}