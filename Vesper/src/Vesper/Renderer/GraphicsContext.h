#pragma once

#include "Vesper/Core/Base.h"

namespace Vesper {

	/// @class GraphicsContext
	/// @brief Abstract class representing a graphics context.
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() {}
		/// @brief Initializes the graphics context.
		virtual void Init() = 0;
		/// @brief Swaps the front and back buffers.
		virtual void SwapBuffers() = 0;

	};

}
