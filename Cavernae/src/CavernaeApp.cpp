#include <Vesper.h>
#include <Vesper/App/EntryPoint.h>

#include "Layers/EditorLayer.h"

namespace Vesper {

	class Cavernae : public Vesper::Application
	{
	public:

		Cavernae() 
			: Application("Cavernae")
		{

			PushLayer(new EditorLayer());
		}

		Cavernae(ApplicationSettings settings)
			: Application(settings)
		{
			PushLayer(new EditorLayer());
		}

		~Cavernae() {

		}
	};

	Application* CreateApplication()
	{
		/// Default Editor path
		//return new Cavernae();


		/// Custom Editor path
		ApplicationSettings settings;
		settings.ApplicationName = "Cavernae";

		settings.ProjectDirectory = "";
		settings.AssetsDirectory = ("Cavernae/assets");
		settings.ResourcesDirectory = ("Vesper/src/Resources");

		settings.RendererAPIType = RendererAPI::API::OpenGL;
		settings.Width = 1600;
		settings.Height = 900;
		settings.Mode = WindowMode::Windowed;
		settings.EnableImGui = true;
		settings.EnableVSync = true;
		
		return new Cavernae(settings);
	}

}