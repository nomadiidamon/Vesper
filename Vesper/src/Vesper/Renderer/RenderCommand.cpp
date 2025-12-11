#include "vzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Vesper {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}