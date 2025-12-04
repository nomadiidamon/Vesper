#include "vzpch.h"
#include "Application.h"

#include "Vesper/Events/ApplicationEvent.h"
#include "Vesper/Log.h"


namespace Vesper {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		VZ_TRACE(e);

		while (true) {

		};
	}

}