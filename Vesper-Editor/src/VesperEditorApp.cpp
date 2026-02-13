#include <Vesper.h>
#include <Vesper/App/EntryPoint.h>

#include "Layers/EditorLayer.h"

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