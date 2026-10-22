#include "vzpch.h"
#include "ImGuiLayer_OpenGL.h"

#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Vesper/App/Application.h"

#include <GLFW/glfw3.h>

namespace Vesper {

	ImGuiLayer_OpenGL::ImGuiLayer_OpenGL() : ImGuiLayer()
	{
	}

	ImGuiLayer_OpenGL::~ImGuiLayer_OpenGL()
	{
	}

	void ImGuiLayer_OpenGL::OnAttach()
	{
		VZ_PROFILE_FUNCTION();
		ImGuiLayer::OnAttach();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer_OpenGL::OnDetach()
	{
		VZ_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGuiLayer::OnDetach();
	}

	void ImGuiLayer_OpenGL::Begin()
	{
		VZ_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGuiLayer::Begin();
	}

	void ImGuiLayer_OpenGL::End()
	{
		VZ_PROFILE_FUNCTION();
		ImGuiLayer::End();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}
