#pragma once
#include "../Core/Base.h"
#include "Window.h"
#include "Vesper/App/LayerStack.h"
#include "Vesper/Events/Event.h"
#include "Vesper/Events/ApplicationEvent.h"
#include "Vesper/Core/Timestep.h"
#include "Vesper/ImGui/ImGuiLayer.h"

#include "Vesper/Renderer/RendererAPI.h"

namespace Vesper {


	/// WIP
	enum class WindowMode
	{
		Windowed = 0,
		Fullscreen = 1,
		Borderless = 2
	};

	/// WIP
	struct ApplicationSettings {
		std::string ApplicationName = "Vesper Application";
		std::string WorkingDirectory;
		RendererAPI::API RendererAPI = RendererAPI::API::OpenGL;
		uint32_t Width = 1280;
		uint32_t Height = 720;
		WindowMode Mode = WindowMode::Windowed;
		bool EnableImGui = true;
		bool EnableVSync = false;
	};


	class Application
	{
	public:
		Application(const std::string& name = "");
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
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

