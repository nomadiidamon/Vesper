#pragma once

#include "Vesper/Renderer/OrthographicCamera.h"

#include "Vesper/Renderer/Texture.h"
#include "Vesper/Renderer/SubTexture2D.h"

#include "Vesper/Renderer/Camera.h"
#include "Vesper/Renderer/EditorCamera.h"
#include "Vesper/Scene/Components.h"

namespace Vesper {


	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform); 
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera); // TODO: Remove once we have a proper scene system
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuadWithTexture(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);

		static void DrawQuadWithTexture(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4 tintColor);

		static void DrawQuadRotated(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotationRads, const glm::vec4& color);
		static void DrawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotationRads, const glm::vec4& color);

		static void DrawQuadRotatedWithTexture(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadRotatedWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadRotatedWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);

		static void DrawQuadRotatedWithTexture(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadRotatedWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadRotatedWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);

		//static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& src, int entityID);
		//static void DrawSprite(const glm::mat4& transform, const SubTextureComponent& stc, int entityID);

		static Ref<Texture2D> GetWhiteTexture();

		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
		static void StartBatch();
	};

}
