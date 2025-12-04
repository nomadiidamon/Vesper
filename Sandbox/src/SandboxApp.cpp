#include <Vesper.h>

class ExampleLayer : public Vesper::Layer
{
	public:
	ExampleLayer() 
		: Layer("Example") 
	{
	}
	void OnUpdate() override 
	{
		VZ_INFO("ExampleLayer::Update");
	}
	void OnEvent(Vesper::Event& event) override 
	{
		VZ_TRACE("ExampleLayer::OnEvent: {0}", event);
	}
};


class SandboxApp : public Vesper::Application
{
public:
	SandboxApp() {

		PushLayer(new ExampleLayer());
		PushOverlay(new Vesper::ImGuiLayer());
	}
	~SandboxApp() {

	}
};

Vesper::Application* Vesper::CreateApplication()
{
	return new SandboxApp();
}