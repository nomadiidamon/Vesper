#pragma once

#include "Vesper/Core/Base.h"

namespace Vesper {

	class VESPER_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	};

}
