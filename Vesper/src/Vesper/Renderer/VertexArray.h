#pragma once

#include <memory>
#include "Vesper/Renderer/Buffer.h"

namespace Vesper {

	/// @class VertexArray
	/// @brief An abstraction for a vertex array object (VAO).
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		/// @brief Adds a vertex buffer to the vertex array.
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		/// @brief Sets the index buffer for the vertex array.
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
}