#pragma once

#include "Vesper/Renderer/OrthographicCamera.h"

#include "Vesper/Renderer/Texture.h"
#include "Vesper/Renderer/SubTexture2D.h"

#include "Vesper/Renderer/Camera.h"
#include "Vesper/Renderer/EditorCamera.h"
#include "Vesper/Scene/Components.h"

namespace Vesper {

	/// @class Renderer2D
	/// @brief A 2D renderer for drawing quads and sprites.
	class Renderer2D
	{
	public:
		/// @brief Initializes the 2D renderer.
		static void Init();
		/// @brief Shuts down the 2D renderer.
		static void Shutdown();
		
		/// @brief Begins a new scene with the given camera and transform.
		///
		/// @param camera The camera to use for the scene.
		/// @param transform The transform matrix for the camera.
		static void BeginScene(const Camera& camera, const glm::mat4& transform); 

		/// @brief Begins a new scene with the given editor camera.
		///
		/// @param camera The editor camera to use for the scene.
		static void BeginScene(const EditorCamera& camera);

		/// @brief Begins a new scene with the given orthographic camera.
		///
		/// @param camera The orthographic camera to use for the scene.
		/// @todo Remove once we have a proper scene system
		static void BeginScene(const OrthographicCamera& camera);

		/// @brief Ends the current scene.
		static void EndScene();

		/// @brief Flushes the current batch of rendering commands.
		static void Flush();

		/// @brief Draws a colored quad with the given transform and color.
		///
		/// @param transform The transform matrix for the quad.
		/// @param color The color of the quad.
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		/// @brief Draws a colored quad at the specified position and size.
		///
		/// @param position The position of the quad in 2D space.
		/// @param size The size of the quad.
		/// @param color The color of the quad.
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

		/// @brief Draws a colored quad at the specified position and size.
		///
		/// @param position The position of the quad in 3D space.
		/// @param size The size of the quad.
		/// @param color The color of the quad.
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		
		/// @brief Draws a textured quad with the given transform, texture, tiling factor, and tint color.
		///
		/// @param transform The transform matrix for the quad.
		/// @param texture The texture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.	
		static void DrawQuadWithTexture(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);
		
		/// @brief Draws a textured quad at the specified position and size.
		///
		/// @param position The position of the quad in 2D space.
		/// @param size The size of the quad.
		/// @param texture The texture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);
		
		/// @brief Draws a textured quad at the specified position and size.
		///
		/// @param position The position of the quad in 3D space.
		/// @param size The size of the quad.
		/// @param texture The texture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);

		/// @brief Draws a textured quad with the given transform, subtexture, tiling factor, and tint color.
		///
		/// @param transform The transform matrix for the quad.
		/// @param subtexture The subtexture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadWithTexture(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4 tintColor);
		
		/// @brief Draws a textured quad at the specified position and size.
		///
		/// @param position The position of the quad in 2D space.
		/// @param size The size of the quad.
		/// @param subtexture The subtexture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4 tintColor);
		
		/// @brief Draws a textured quad at the specified position and size.
		///
		/// @param position The position of the quad in 3D space.
		/// @param size The size of the quad.
		/// @param subtexture The subtexture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4 tintColor);

		/// @brief Draws a rotated colored quad with the given transform and color.
		///
		/// @param transform The transform matrix for the quad.
		/// @param color The color of the quad.
		static void DrawQuadRotated(const glm::mat4& transform, const glm::vec4& color);

		/// @brief Draws a rotated colored quad at the specified position, size, and rotation.
		///
		/// @param position The position of the quad in 2D space.
		/// @param size The size of the quad.
		/// @param rotationRads The rotation of the quad in radians.
		/// @param color The color of the quad.
		static void DrawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotationRads, const glm::vec4& color);

		/// @brief Draws a rotated colored quad at the specified position, size, and rotation.
		///
		/// @param position The position of the quad in 3D space.
		/// @param size The size of the quad.
		/// @param rotationRads The rotation of the quad in radians.
		/// @param color The color of the quad.
		static void DrawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotationRads, const glm::vec4& color);

		/// @brief Draws a rotated textured quad with the given transform, texture, tiling factor, and tint color.
		///
		/// @param transform The transform matrix for the quad.
		/// @param texture The texture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadRotatedWithTexture(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 tintColor);
		
		/// @brief Draws a rotated textured quad at the specified position, size, and rotation.
		///
		/// @param position The position of the quad in 2D space.
		/// @param size The size of the quad.
		/// @param rotationRads The rotation of the quad in radians.
		/// @param texture The texture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadRotatedWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);
		
		/// @brief Draws a rotated textured quad at the specified position, size, and rotation.
		///
		/// @param position The position of the quad in 3D space.
		/// @param size The size of the quad.
		/// @param rotationRads The rotation of the quad in radians.
		/// @param texture The texture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadRotatedWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);

		/// @brief Draws a rotated textured quad with the given transform, subtexture, tiling factor, and tint color.
		///
		/// @param transform The transform matrix for the quad.
		/// @param subtexture The subtexture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadRotatedWithTexture(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4 tintColor);
		
		/// @brief Draws a rotated textured quad at the specified position, size, and rotation.
		///
		/// @param position The position of the quad in 2D space.
		/// @param size The size of the quad.
		/// @param rotationRads The rotation of the quad in radians.
		/// @param subtexture The subtexture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadRotatedWithTexture(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);
		
		/// @brief Draws a rotated textured quad at the specified position, size, and rotation.
		///
		/// @param position The position of the quad in 3D space.
		/// @param size The size of the quad.
		/// @param rotationRads The rotation of the quad in radians.
		/// @param subtexture The subtexture to apply to the quad.
		/// @param tilingFactor The tiling factor for the texture.
		/// @param tintColor The tint color to apply to the texture.
		static void DrawQuadRotatedWithTexture(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float rotationRads, float tilingFactor, const glm::vec4 tintColor);


		//static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& src, int entityID);
		//static void DrawSprite(const glm::mat4& transform, const SubTextureComponent& stc, int entityID);

		/// @brief Returns a reference to the default white texture that allows for coloring.
		static Ref<Texture2D> GetWhiteTexture();

		/// 2D Renderer Statistics
		struct Statistics {
			/// @brief The number of draw calls being made
			uint32_t DrawCalls = 0;
			/// @brief The number of quads being drawn
			uint32_t QuadCount = 0;
			/// @brief Calculates the total number of vertices drawn
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			/// @brief Calculates the total number of indices drawn
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		/// @brief Resets the rendering statistics.
		static void ResetStats();
		/// @brief Retrieves the current rendering statistics.
		static Statistics GetStats();

	private:
		static void FlushAndReset();
		static void StartBatch();
	};

}
