#pragma once
#include "Base.h"
#include "Window.h"
#include "Vesper/Core/LayerStack.h"
#include "Vesper/Events/Event.h"
#include "Vesper/Events/ApplicationEvent.h"
#include "Vesper/Core/Timestep.h"

#include "Vesper/ImGui/ImGuiLayer.h"

namespace Vesper {

	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

