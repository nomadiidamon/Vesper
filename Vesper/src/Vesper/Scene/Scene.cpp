#include "vzpch.h"
#include "Scene.h"
#include "Vesper/Core/Log.h"
#include "Vesper/Renderer/Renderer2D.h"
#include "Vesper/Scene/Entity.h"
#include "Vesper/Scene/ScriptableEntity.h"
#include "Vesper/Renderer/EditorCamera.h"
#include "Vesper/Input/Input.h"
#include "Vesper/Input/KeyCodes.h"
#include "Vesper/Input/MouseButtonCodes.h"

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


	void Scene::OnUpdateRuntime(Timestep ts)
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
				Renderer2D::DrawQuad(transform.GetTransform(), VZ_DEFAULT_TEXTURE, sprite.TilingFactor, sprite.Color);
			else if (sprite.TextureEnabled && sprite.Texture)
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.TilingFactor, sprite.Color);
			else
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
		}

		auto subTextureView = m_Registry.group<SubTextureComponent>();
		for (auto entity : subTextureView) {
			auto& transform = m_Registry.get<TransformComponent>(entity);
			auto& sprite = m_Registry.get<SpriteRendererComponent>(entity);
			auto& subTexture = m_Registry.get<SubTextureComponent>(entity);

			auto texAnim = m_Registry.try_get<TextureAnimationComponent>(entity);
			if (texAnim) {
				continue;
				// Do not render texture animations here
			}

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

			Renderer2D::DrawQuad(transform.GetTransform(), subTexture.GetSubTexture(), sprite.TilingFactor, sprite.Color);
		}

		auto texAnimGroup = m_Registry.group<TextureAnimationComponent>();
		for (auto entity : texAnimGroup)
		{
			auto& transform = m_Registry.get<TransformComponent>(entity);
			auto& texAnim = m_Registry.get<TextureAnimationComponent>(entity);

			auto sprite = m_Registry.try_get<SpriteRendererComponent>(entity);
			if (sprite && sprite->Texture && sprite->TextureEnabled) {

				auto subTex = m_Registry.try_get<SubTextureComponent>(entity);
				if (subTex && subTex->SubTexture &&
					subTex->SubTexture->GetTexture() &&
					subTex->SubTexture->GetTexture() == sprite->Texture) {

					if (texAnim.SubTextures.empty()) {
						texAnim.SubTextures.push_back(subTex->SubTexture);
						texAnim.CurrentFrame = 0;
						texAnim.FrameTime = 0.16f; // Default frame time, can be adjusted as needed
						texAnim.TimeAccumulator = 0.0f;
						texAnim.tintColor = sprite->Color;
						continue;
					}
					texAnim.Update(ts.GetSeconds());
					Renderer2D::DrawQuad(transform.GetTransform(), texAnim.SubTextures[texAnim.CurrentFrame], 1.0f, texAnim.tintColor);
				}
			}
		}


		{
			VZ_PROFILE_SCOPE("Particle System(s) Update");

			auto particleSystemView = m_Registry.group<ParticleSystemComponent>();
			for (auto entity : particleSystemView) {
				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& particleSystem = m_Registry.get<ParticleSystemComponent>(entity);
				auto& ps = particleSystem.ParticleSystem;
				auto& props = ps.m_Props;
				props.Position = transform.Translation;
				ps.OnUpdate(ts);

				if (ps.m_IsEmitting) {
					for (int i = 0; i < ps.m_EmitRate; i++) {
						ps.Emit(ps.m_Props);
					}
					ps.m_IsEmitting = false;
				}

				ps.OnRender();
			}
		}

		Renderer2D::EndScene();

	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		VZ_PROFILE_FUNCTION();
		Renderer2D::BeginScene(camera);
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
				Renderer2D::DrawQuad(transform.GetTransform(), VZ_DEFAULT_TEXTURE, sprite.TilingFactor, sprite.Color);
			else if (sprite.TextureEnabled && sprite.Texture)
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.TilingFactor, sprite.Color);
			else
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
		}
		auto subTextureView = m_Registry.group<SubTextureComponent>();
		for (auto entity : subTextureView) {
			auto& transform = m_Registry.get<TransformComponent>(entity);
			auto& sprite = m_Registry.get<SpriteRendererComponent>(entity);
			auto& subTexture = m_Registry.get<SubTextureComponent>(entity);
			auto texAnim = m_Registry.try_get<TextureAnimationComponent>(entity);
			if (texAnim) {
				continue;
				// Do not render texture animations here
			}
			if (!sprite.TextureEnabled) sprite.TextureEnabled = true;
			if (subTexture.SubTexture == nullptr)
			{
				subTexture.SetTexture(VZ_DEFAULT_TEXTURE);
			}
			else {
				// TODO: find way to avoid this check every frame
				// Ensure the subtexture's texture matches the sprite's texture
				if (sprite.Texture && subTexture.SubTexture->GetTexture() != sprite.Texture) {
					subTexture.SetTexture(sprite.Texture);
				}
			}
			Renderer2D::DrawQuad(transform.GetTransform(), subTexture.GetSubTexture(), sprite.TilingFactor, sprite.Color);
		}

		auto texAnimGroup = m_Registry.group<TextureAnimationComponent>();
		for (auto entity : texAnimGroup)
		{
			auto& name = m_Registry.get<NameComponent>(entity).Name;
			auto& transform = m_Registry.get<TransformComponent>(entity);
			auto& texAnim = m_Registry.get<TextureAnimationComponent>(entity);

			auto sprite = m_Registry.try_get<SpriteRendererComponent>(entity);
			if (sprite && sprite->Texture && sprite->TextureEnabled) {

				auto subTex = m_Registry.try_get<SubTextureComponent>(entity);
				if (subTex && subTex->SubTexture) {

					if (subTex->SubTexture->GetTexture() &&
						subTex->SubTexture->GetTexture() == sprite->Texture) {

						if (texAnim.SubTextures.empty()) {
							texAnim.SubTextures.push_back(subTex->SubTexture);
							texAnim.CurrentFrame = 0;
							texAnim.FrameTime = 0.16f; // Default frame time, can be adjusted as needed
							texAnim.TimeAccumulator = 0.0f;
							texAnim.tintColor = sprite->Color;
							continue;
						}
						texAnim.tintColor = sprite->Color;
						texAnim.Update(ts.GetSeconds());
						Renderer2D::DrawQuad(transform.GetTransform(), texAnim.SubTextures[texAnim.CurrentFrame], 1.0f, texAnim.tintColor);

					}
					else {

						if (subTex->SubTexture->GetTexture()) {
							//VZ_CORE_WARN("Entity %s has SubTextureComponent with a texture that does not match the SpriteRendererComponent's texture. Animation will not play.", name);
							subTex->SubTexture->SetTexture(sprite->Texture);
							subTex->SubTexture = SubTexture2D::CreateFromCoords(sprite->Texture, { 0, 0 }, { sprite->Texture->GetWidth(), sprite->Texture->GetHeight() });
						}
						else {
							//VZ_CORE_WARN("Entity %s has SubTextureComponent with no texture. Animation will not play.", name);
							subTex->SubTexture->SetTexture(sprite->Texture);
							subTex->SubTexture = SubTexture2D::CreateFromCoords(sprite->Texture, { 0, 0 }, { sprite->Texture->GetWidth(), sprite->Texture->GetHeight() });

						}

					}
				}
				else {

					if (subTex) {
						VZ_CORE_WARN("Entity %s has SubTextureComponent but it has no SubTexture. Animation will not play.", name);
						m_Registry.emplace_or_replace<SubTextureComponent>(entity, sprite->Texture);

					}
					else {
						VZ_CORE_WARN("Entity %s has no SubTextureComponent. Animation will not play.", name);
						m_Registry.emplace<SubTextureComponent>(entity, sprite->Texture);

					}
				}
			}
			else {
				if (sprite) {
					if (!sprite->Texture) {
						VZ_CORE_WARN("Entity %s has SpriteRendererComponent with no texture. Animation will not play.", name);
						//sprite->Texture = Renderer2D::GetWhiteTexture();
					}

					if (!sprite->TextureEnabled) {
						sprite->TextureEnabled = true;
					}

					texAnim.Texture = sprite->Texture;
					texAnim.tintColor = sprite->Color;
				}
				else {
					VZ_CORE_WARN("Entity %s has no SpriteRendererComponent. Animation will not play.", name);
				}
			}
		}

		{
			VZ_PROFILE_SCOPE("Particle System(s) Update");

			auto particleSystemView = m_Registry.group<ParticleSystemComponent>();
			for (auto entity : particleSystemView) {
				auto& transform = m_Registry.get<TransformComponent>(entity);
				auto& particleSystem = m_Registry.get<ParticleSystemComponent>(entity);
				auto& ps = particleSystem.ParticleSystem;
				auto& props = ps.m_Props;
				props.Position = transform.Translation;
				ps.OnUpdate(ts);
				if (ps.m_IsEmitting) {
					for (int i = 0; i < ps.m_EmitRate; i++) {
						ps.Emit(ps.m_Props);
					}
					ps.m_IsEmitting = false;
				}

				ps.OnRender();
			}
		}

		{
			auto shadowCloneView = m_Registry.group<ShadowCloneComponent>();
			for (auto entity : shadowCloneView) {
				UUID& originalEntityID = m_Registry.get<UUIDComponent>(entity).ID;
				auto& shadowClone = m_Registry.get<ShadowCloneComponent>(entity);

				if (shadowClone.CloneEntities.empty()) {
					// If there are no clones, create them
					for (int i = 0; i < shadowClone.NumberOfClones; i++) {
						Entity cloneEntity = CreateEntity("Shadow Clone " + std::to_string(i + 1));
						
						auto& transform = cloneEntity.GetComponent<TransformComponent>();
						int order = shadowClone.CloneEntities.size();
						transform.Translation = { transform.Translation.x + shadowClone.PositionVariation.x * order, transform.Translation.y + shadowClone.PositionVariation.y * order, transform.Translation.z };

						cloneEntity.AddComponent<CloneBehaviorComponent>();
						cloneEntity.GetComponent<CloneBehaviorComponent>().OriginalEntityID = originalEntityID;
						shadowClone.CloneEntities.push_back(cloneEntity.GetComponent<UUIDComponent>().ID);
					}
				}
				else if (shadowClone.NumberOfClones != shadowClone.CloneEntities.size()) {
					// If the number of clones has changed, add or remove clones accordingly
					int currentClones = static_cast<int>(shadowClone.CloneEntities.size());
					if (shadowClone.NumberOfClones > currentClones) {
						for (int i = currentClones; i < shadowClone.NumberOfClones; i++) {
							Entity cloneEntity = CreateEntity("Shadow Clone " + std::to_string(i + 1));

							auto& transform = cloneEntity.GetComponent<TransformComponent>();
							int order = shadowClone.CloneEntities.size();
							transform.Translation = { transform.Translation.x + shadowClone.PositionVariation.x * order, transform.Translation.y + shadowClone.PositionVariation.y * order, transform.Translation.z };

							cloneEntity.AddComponent<CloneBehaviorComponent>();
							cloneEntity.GetComponent<CloneBehaviorComponent>().OriginalEntityID = originalEntityID;
							shadowClone.CloneEntities.push_back(cloneEntity.GetComponent<UUIDComponent>().ID);
						}
					}
					else {
						for (int i = currentClones - 1; i >= shadowClone.NumberOfClones; i--) {
							Entity cloneEntity = GetEntityFromUUID(shadowClone.CloneEntities[i]);
							if (cloneEntity) {
								DestroyEntity(cloneEntity);
							}
							shadowClone.CloneEntities.pop_back();
						}
					}
				}
				for (auto& cloneID : shadowClone.CloneEntities) {
					Entity cloneEntity = GetEntityFromUUID(cloneID);
					if (cloneEntity) {
						if (shadowClone.SyncWithOriginal) {
							shadowClone.Independent = false; // Ensure independent is false if syncing

							auto& originalTransform = m_Registry.get<TransformComponent>(entity);
							auto& cloneTransform = cloneEntity.GetComponent<TransformComponent>();
							cloneTransform.Translation = originalTransform.Translation + shadowClone.PositionVariation;
							cloneTransform.Rotation = originalTransform.Rotation;
							cloneTransform.Scale = originalTransform.Scale;
						}
						else if (shadowClone.Independent) {
							shadowClone.SyncWithOriginal = false; // Ensure syncing is false if independent

							if (cloneEntity.HasComponent<CloneBehaviorComponent>()) {
								auto& cloneBehavior = cloneEntity.GetComponent<CloneBehaviorComponent>();
								
								/// Should do this in on add. just testing for now
								if (!cloneBehavior.CustomUpdateFunction) {
									cloneBehavior.SetCustomUpdateFunction([](Scene* context, UUID originalEntityID, UUID cloneEntityID, float deltaTime, float orbitSpeed, float orbitRadius) {
										// Custom behavior for independent shadow clones
										// For example, we can make the clone orbit around the original entity
										Entity originalEntity = context->GetEntityFromUUID(originalEntityID);
										Entity cloneEntity = context->GetEntityFromUUID(cloneEntityID);
										auto& originalTransform = originalEntity.GetComponent<TransformComponent>();
										auto& cloneTransform = cloneEntity.GetComponent<TransformComponent>();
										// Calculate the angle based on time
										static float angle = 0.0f;
										angle += orbitSpeed * deltaTime;
										// Update the clone's position to orbit around the original entity
										cloneTransform.Translation.x = originalTransform.Translation.x + orbitRadius * cos(angle);
										cloneTransform.Translation.y = originalTransform.Translation.y + orbitRadius * sin(angle);
										}
									);
								}
								else {
									cloneBehavior.CloneBehaviorUpdate(this, originalEntityID, cloneID, ts, cloneBehavior.orbitSpeed, cloneBehavior.orbitRadius);
								}


							}
							else {
								VZ_CORE_WARN("Entity %s has Independent Shadow Clone but no CloneBehaviorComponent. Clone will not update.", cloneEntity.GetName().c_str());
							}
						}
					}
				}
			}
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

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {

			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityFromUUID(const UUID& id)
	{
		auto view = m_Registry.view<UUIDComponent>();
		for (auto entity : view) {
			const auto& uuid = view.get<UUIDComponent>(entity);
			if (uuid.ID.ID == id.ID)
				return Entity{ entity, this };
		}
		return {};
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
		component.Texture = Renderer2D::GetWhiteTexture();
	}

	template<>
	void Scene::OnComponentAdded<SubTextureComponent>(Entity entity, SubTextureComponent& component) {
		auto& src = entity.GetOrAddComponent<SpriteRendererComponent>();
		//if (!src.TextureEnabled) src.TextureEnabled = true;
		component.SetTexture(src.Texture ? src.Texture : Renderer2D::GetWhiteTexture());
		component.SubTexture = SubTexture2D::CreateFromCoords(src.Texture, { 0,0 }, { src.Texture->GetWidth(), src.Texture->GetHeight() });
	}

	template<>
	void Scene::OnComponentAdded<TextureAnimationComponent>(Entity entity, TextureAnimationComponent& component) {
		auto& sprc = entity.GetOrAddComponent<SpriteRendererComponent>();
		auto& stc = entity.GetOrAddComponent<SubTextureComponent>();
		stc.SetTexture(sprc.Texture);
		component.Texture = sprc.Texture;
		//if (sprc.Texture) {
		//	if (!stc.SubTexture) {
		//		stc.SetTexture(sprc.Texture);
		//		stc.SubTexture = SubTexture2D::CreateFromCoords(sprc.Texture, { 0, 0 }, { sprc.Texture->GetWidth(), sprc.Texture->GetHeight() });
		//	}
		//	else if (stc.SubTexture->GetTexture() != sprc.Texture) {
		//		stc.SetTexture(sprc.Texture);
		//		stc.SubTexture = SubTexture2D::CreateFromCoords(sprc.Texture, { 0, 0 }, { sprc.Texture->GetWidth(), sprc.Texture->GetHeight() });
		//	}
		//}
		//else if (stc.SubTexture) {
		//	sprc.Texture = stc.SubTexture->GetTexture();
		//}
		//else {
		//	stc.SetTexture(Renderer2D::GetWhiteTexture());

		//}
		//if (sprc.Texture)
		//	component.Texture = sprc.Texture;
		//else if (stc.SubTexture)
		//	component.Texture = stc.SubTexture->GetTexture();
		//else
		//	component.Texture = Renderer2D::GetWhiteTexture();
	}


	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded<ParticleSystemComponent>(Entity entity, ParticleSystemComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded<ShadowCloneComponent>(Entity entity, ShadowCloneComponent& component) {
		for (int i = 0; i < component.NumberOfClones; i++) {
			Entity clone = CreateEntity(entity.GetComponent<NameComponent>().Name + "_Clone" + std::to_string(i));
			//clone.Duplicate(entity);
			clone.GetComponent<TransformComponent>() = entity.GetComponent<TransformComponent>();
			clone.GetComponent<TransformComponent>().Translation += glm::vec3(component.PositionVariation.x * static_cast<float>(i), component.PositionVariation.y * static_cast<float>(i), component.PositionVariation.z * static_cast<float>(i));

			if (entity.HasComponent<SpriteRendererComponent>()) {
				clone.AddComponent<SpriteRendererComponent>(entity.GetComponent<SpriteRendererComponent>());
			}
			if (entity.HasComponent<SubTextureComponent>()) {
				clone.AddComponent<SubTextureComponent>(entity.GetComponent<SubTextureComponent>());
			}
			if (entity.HasComponent<TextureAnimationComponent>()) {
				clone.AddComponent<TextureAnimationComponent>(entity.GetComponent<TextureAnimationComponent>());
			}

			auto& cloneBehavior = clone.AddComponent<CloneBehaviorComponent>();
			cloneBehavior.OriginalEntityID = entity.GetID();
			cloneBehavior.orbitRadius *= (i + 1);
			//cloneBehavior.orbitSpeed *= (i + 1);
			component.CloneEntities.push_back(clone.GetID());
		}
	}

	template<>
	void Scene::OnComponentAdded<CloneBehaviorComponent>(Entity entity, CloneBehaviorComponent& component) {
	
		component.SetCustomUpdateFunction([](Scene* context, UUID originalEntityID, UUID cloneEntityID, float deltaTime, float orbitSpeed, float orbitRadius) {
			// Custom behavior for independent shadow clones
			// For example, we can make the clone orbit around the original entity
			Entity originalEntity = context->GetEntityFromUUID(originalEntityID);
			Entity cloneEntity = context->GetEntityFromUUID(cloneEntityID);
			auto& originalTransform = originalEntity.GetComponent<TransformComponent>();
			auto& cloneTransform = cloneEntity.GetComponent<TransformComponent>();
			// Calculate the angle based on time
			static float angle = 0.0f;
			angle += orbitSpeed * deltaTime;
			// Update the clone's position to orbit around the original entity
			cloneTransform.Translation.x = originalTransform.Translation.x + orbitRadius * cos(angle);
			cloneTransform.Translation.y = originalTransform.Translation.y + orbitRadius * sin(angle);
			}
		);


	}
}