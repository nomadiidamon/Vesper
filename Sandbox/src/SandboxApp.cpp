#include <Vesper.h>

#include "imgui/imgui.h"



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

	void OnImGuiRender() override 
	{
		ImGui::Begin("Example");
		ImGui::Text("Hello from ImGui!");
		ImGui::End();
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
	}
	~SandboxApp() {

	}
};

Vesper::Application* Vesper::CreateApplication()
{
	return new SandboxApp();
}