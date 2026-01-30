#pragma once

#include "Vesper/Renderer/RenderCommand.h"
#include "Vesper/Renderer/OrthographicCamera.h"
#include "Vesper/Renderer/Shader.h"

namespace Vesper {
	
	/// @class Renderer
	/// @brief The main renderer class responsible for managing rendering operations.
	class Renderer {

	public:

		/// @brief Initializes the renderer.
		static void Init();

		/// @brief Handles window resize events.
		static void OnWindowResize(uint32_t width, uint32_t height);


		/// @brief Begins a new scene with the given orthographic camera.
		static void BeginScene(OrthographicCamera& camera);
		/// @brief Ends the current scene.
		static void EndScene();

		/// @brief Submits a draw call with the specified shader, vertex array, and transform.
		///
		/// @param shader The shader to use for rendering.
		/// @param vertexArray The vertex array to draw.
		/// @param transform The transformation matrix to apply.
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		/// @brief Retrieves the current rendering API.
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		/// @brief Scene data structure containing view-projection matrix.
		struct SceneData {
			/// @brief The combined view-projection matrix for the scene.
			glm::mat4 ViewProjectionMatrix;
		};

		/// @brief Pointer to the current scene data.
		static SceneData* s_SceneData;

	};
}