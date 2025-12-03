#pragma once

#ifdef VZ_PLATFORM_WINDOWS

extern Vesper::Application* Vesper::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Vesper::CreateApplication();
	app->Run();
	delete app;
}

#endif