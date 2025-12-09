#include "vzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Vesper/Renderer/GraphicsContext.h"

namespace Vesper {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
		: m_WindowHandle(windowHandle)
	{
		VZ_CORE_ASSERT(windowHandle, "Window handle is null!");

	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		VZ_CORE_INFO("OpenGL Info:");
		VZ_CORE_INFO("  Vendor: {0}", (const char *)glGetString(GL_VENDOR));
		VZ_CORE_INFO("  Renderer: {0}", (const char *)glGetString(GL_RENDERER));
		VZ_CORE_INFO("  Version: {0}", (const char *)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);

	}

}