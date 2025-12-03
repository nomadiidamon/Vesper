#include <Vesper.h>

class SandboxApp : public Vesper::Application
{
public:
	SandboxApp() {

	}
	~SandboxApp() {

	}
};

Vesper::Application* Vesper::CreateApplication()
{
	return new SandboxApp();
}