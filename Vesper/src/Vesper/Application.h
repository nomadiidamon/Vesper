#pragma once
#include "Core.h"
#include "Window.h"
#include "Vesper/LayerStack.h"
#include "Vesper/Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Vesper/ImGui/ImGuiLayer.h"

#include "Vesper/Renderer/Shader.h"
#include "Vesper/Renderer/Buffer.h"
#include "Vesper/Renderer/VertexArray.h"

namespace Vesper {

	class VESPER_API Application
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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		//unsigned int m_VertexArray;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> m_Shader;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_BlueShader;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

