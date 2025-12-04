#include "vzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include <GLFW/glfw3.h>
#include "Vesper/Application.h"

namespace  Vesper {
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		if (!ImGui::GetCurrentContext())
			ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

		// Guard against initializing the renderer backend more than once.
		// ImGui_ImplOpenGL3_Init asserts if io.BackendRendererUserData != nullptr.
		if (io.BackendRendererUserData == nullptr)
		{
			ImGui_ImplOpenGL3_Init("#version 410");
		}
		else
		{
			VZ_CORE_WARN("ImGui renderer backend already initialized — skipping ImGui_ImplOpenGL3_Init()");
		}
	}

	void ImGuiLayer::OnDetach()
	{
		// ImGui_ImplOpenGL3_Shutdown() is safe to call only if the backend was initialized.
		ImGuiIO& io = ImGui::GetIO();
		if (io.BackendRendererUserData != nullptr)
			ImGui_ImplOpenGL3_Shutdown();

		// Destroy the ImGui context only if it's the current context.
		if (ImGui::GetCurrentContext())
			ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetApplication();	
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}