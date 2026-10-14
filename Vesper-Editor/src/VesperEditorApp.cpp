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

		VesperEditor(ApplicationSettings settings)
			: Application(settings)
		{
			PushLayer(new EditorLayer());
		}

		~VesperEditor() {

		}
	};

	Application* CreateApplication()
	{
		/// Default Editor path
		//return new VesperEditor();


		/// Custom Editor path
		ApplicationSettings settings;
		settings.ApplicationName = "Caverna";

		settings.ProjectDirectory = "";
		settings.AssetsDirectory = ("Vesper-Editor/assets");
		settings.ResourcesDirectory = ("Vesper/src/Resources");

		settings.RendererAPIType = RendererAPI::API::OpenGL;
		settings.Width = 1600;
		settings.Height = 900;
		settings.Mode = WindowMode::Windowed;
		settings.EnableImGui = true;
		settings.EnableVSync = true;
		
		return new VesperEditor(settings);
	}

}