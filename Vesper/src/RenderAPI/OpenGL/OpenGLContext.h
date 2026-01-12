#pragma once

#include "Vesper/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Vesper {
	class VESPER_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual ~OpenGLContext();
		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}