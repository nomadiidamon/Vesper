#include <Vesper/ImGui/VesperImGui.h>

#include "Sandbox2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Vesper::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Vesper::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);


	// Render
	Vesper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vesper::RenderCommand::Clear();

	Vesper::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Vesper::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);
	Vesper::Renderer2D::DrawQuad({ 1.0f, -0.5f, 0.0f }, { 0.5f, 0.75 }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Vesper::Renderer2D::DrawQuadTextured({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, m_textureScale, m_TextureTintColor);
	Vesper::Renderer2D::EndScene();

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	if (ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor))) 
	{

	}
	if (ImGui::ColorEdit4("Texture Tint Color", glm::value_ptr(m_TextureTintColor))) 
	{
	}
	if (ImGui::SliderFloat("Texture Scale", &m_textureScale, 0.1f, 10.0f)) 
	{
	}
	Vesper::DisplayVesperInfo_ImGui();
	ImGui::End();
}

void Sandbox2D::OnEvent(Vesper::Event& e)
{
	m_CameraController.OnEvent(e);
}
