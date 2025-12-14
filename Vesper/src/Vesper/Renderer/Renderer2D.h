#pragma once

#include "Vesper/Renderer/OrthographicCamera.h"

#include "Vesper/Renderer/Texture.h"


namespace Vesper {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuadWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);

		static void DrawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotationRads, const glm::vec4& color);
		static void DrawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotationRads, const glm::vec4& color);
		static void DrawQuadRotatedWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);
		static void DrawQuadRotatedWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);

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
	};

}
