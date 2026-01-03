#pragma once
#include "Vesper/Renderer/Texture.h"
#include "Vesper/Renderer/SubTexture2D.h"
#include "SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Vesper
{
	struct NameComponent
	{
		std::string Name;
		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name)
			: Name(name) {
		}
		operator std::string& () { return Name; }
		operator const std::string& () const { return Name; }
		std::string& GetName() { return Name; }
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture = nullptr;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {
		}

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }

		glm::vec4& GetColor() { return Color; }

	};

	struct TextureComponent
	{
		Ref<Texture2D> Texture;
		TextureComponent() = default;
		TextureComponent(const TextureComponent&) = default;
		TextureComponent(const Ref<Texture2D>& texture)
			: Texture(texture) {
		}

		operator Ref<Texture2D>& () { return Texture; }
		operator const Ref<Texture2D>& () const { return Texture; }
		Ref<Texture2D>& GetTexture() { return Texture; }
	};

	struct SubTextureComponent
	{
		Ref<SubTexture2D> SubTexture;
		SubTextureComponent() = default;
		SubTextureComponent(const SubTextureComponent&) = default;
		SubTextureComponent(const Ref<SubTexture2D>& subTexture)
			: SubTexture(subTexture) {
		}

		operator Ref<SubTexture2D>& () { return SubTexture; }
		operator const Ref<SubTexture2D>& () const { return SubTexture; }
		Ref<SubTexture2D>& GetSubTexture() { return SubTexture; }
	};

	// Animates through a series of sub textures (can be used with full textures)
	struct TextureAnimationComponent
	{
		std::vector<Ref<SubTexture2D>> SubTextures;
		uint32_t CurrentFrame = 0;
		float FrameTime = 0.6f; // Time per frame in seconds
		float TimeAccumulator = 0.0f; // per-instance accumulator

		TextureAnimationComponent() = default;
		TextureAnimationComponent(const TextureAnimationComponent&) = default;
		TextureAnimationComponent(const std::vector<Ref<SubTexture2D>>& subTextures, float frameTime)
			: SubTextures(subTextures), FrameTime(frameTime) {
		}
		operator std::vector<Ref<SubTexture2D>>& () { return SubTextures; }
		operator const std::vector<Ref<SubTexture2D>>& () const { return SubTextures; }

		std::vector<Ref<SubTexture2D>>& GetSubTextures() { return SubTextures; }
		uint32_t GetCurrentFrame() const { return CurrentFrame; }

		void Update(float deltaTime) {
			if (SubTextures.empty() || FrameTime <= 0.0f)
				return;

			TimeAccumulator += deltaTime;
			while (TimeAccumulator >= FrameTime) {
				CurrentFrame = (CurrentFrame + 1) % static_cast<uint32_t>(SubTextures.size());
				TimeAccumulator -= FrameTime;
			}
		}


	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

	};

	class ScriptableEntity;
	class Timestep;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*> (new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}