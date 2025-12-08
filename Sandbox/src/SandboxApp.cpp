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

		if (Vesper::Input::IsKeyPressed(VZ_KEY_TAB))
			VZ_TRACE("Tab key is pressed (poll)!");
	}
	void OnEvent(Vesper::Event& event) override 
	{
		if (event.GetEventType() == Vesper::EventType::KeyPressed) 
		{
			Vesper::KeyPressedEvent& e = static_cast<Vesper::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == VZ_KEY_TAB)
				VZ_TRACE("Tab key pressed (event)!");
			VZ_TRACE("{0} key pressed (repeat={1})", (char)e.GetKeyCode(), e.GetRepeatCount());
		}
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