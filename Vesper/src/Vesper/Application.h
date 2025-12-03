#pragma once
#include "Core.h"


namespace Vesper {

	class VESPER_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

