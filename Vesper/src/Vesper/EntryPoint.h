#pragma once

#ifdef VZ_PLATFORM_WINDOWS

extern Vesper::Application* Vesper::CreateApplication();

int main(int argc, char** argv)
{

	Vesper::Log::Init();
	VZ_CORE_WARN("Starting Vesper Engine...");
	int a = 5;
	VZ_INFO("Vesper Engine Initialized. Var={0}", a);
	VZ_ERROR("This is an error message to test error logging");
	VZ_FATAL("This is a fatal message to test fatal logging");

	auto app = Vesper::CreateApplication();
	app->Run();
	delete app;
}

#endif