#pragma once

#include "Vesper/Renderer/RenderCommand.h"
#include "Vesper/Renderer/OrthographicCamera.h"
#include "Vesper/Renderer/Shader.h"

namespace Vesper {
	
	class Renderer {

	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;

	};
}