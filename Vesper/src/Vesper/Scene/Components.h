#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vesper/Renderer/Texture.h"
#include "Vesper/Renderer/SubTexture2D.h"

namespace Vesper 
{
	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {
		}
		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
		std::string& GetTag() { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {
		}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }

		void Translate(const glm::vec3& translation) {
			Transform = glm::translate(Transform, translation);
		}

		glm::vec3 Translation() const {
			return glm::vec3(Transform[3]);
		}
		
		void Rotate(float angle, const glm::vec3& axis) {
			Transform = glm::rotate(Transform, angle, axis);
		}

		float Rotation(const glm::vec3& axis) const {
			if (axis.x == 1.0f)
				return atan2(Transform[2][1], Transform[2][2]);
			else if (axis.y == 1.0f)
				return atan2(-Transform[2][0], sqrt(Transform[2][1] * Transform[2][1] + Transform[2][2] * Transform[2][2]));
			else if (axis.z == 1.0f)
				return atan2(Transform[1][0], Transform[0][0]);
			VZ_CORE_ASSERT(false, "Rotation axis must be a unit vector along x, y, or z.");
			return 0.0f;
		}

		void Scale(const glm::vec3& scale) {
			Transform = glm::scale(Transform, scale);
		}

		glm::vec3 Scale() const {
			return glm::vec3(
				glm::length(glm::vec3(Transform[0])),
				glm::length(glm::vec3(Transform[1])),
				glm::length(glm::vec3(Transform[2]))

			);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

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

}