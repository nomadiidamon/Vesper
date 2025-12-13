#pragma once

#include "Vesper/Core/Core.h"

namespace Vesper {

	class VESPER_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}
