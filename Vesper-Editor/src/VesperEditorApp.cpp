#include <Vesper.h>
#include <Vesper/Core/EntryPoint.h>


#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

#include "EditorLayer.h"

namespace Vesper {

	class VesperEditor : public Vesper::Application
	{
	public:
		VesperEditor() 
			: Application("Vesper Editor")
		{

			PushLayer(new EditorLayer());
		}

		~VesperEditor() {

		}
	};

	Application* CreateApplication()
	{
		return new VesperEditor();
	}

}