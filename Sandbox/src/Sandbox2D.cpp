#include <Vesper/ImGui/VesperImGui.h>

#include "Sandbox2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
	VZ_PROFILE_FUNCTION();
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	VZ_PROFILE_FUNCTION();
	m_CheckerboardTexture = Vesper::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	VZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Vesper::Timestep ts)
{
	VZ_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	{
		VZ_PROFILE_SCOPE("Renderer Prep");
		// Render
		Vesper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vesper::RenderCommand::Clear();
	}

	{
		VZ_PROFILE_SCOPE("Renderer2D Draw");
		Vesper::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// Checkerboard background
		//Vesper::Renderer2D::DrawQuadTextured({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, m_textureScale, m_TextureTintColor);

		// Squares
		Vesper::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);
		Vesper::Renderer2D::DrawQuad({ 1.0f, -0.5f, 0.0f }, { 0.5f, 0.75 }, { 0.8f, 0.2f, 0.3f, 1.0f });
		
		// Rotated Squares
		//Vesper::Renderer2D::DrawQuadTexturedRotated({ 0.0f, 0.0f, -0.05f }, { 0.5f, 0.5f }, glm::radians(m_squareRotation), m_CheckerboardTexture, m_textureScale, m_TextureTintColor);
		//Vesper::Renderer2D::DrawQuadRotated({ 0.0f, 0.0f, -0.05f }, { 1.0f, 1.0f }, glm::radians(45.0f), m_SquareColor);
		
		Vesper::Renderer2D::EndScene();
	}

}

void Sandbox2D::OnImGuiRender()
{
	VZ_PROFILE_FUNCTION();

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
	if (ImGui::SliderFloat("Square Rotation", &m_squareRotation, 0.0f, 360.0f))
	{
	}
	Vesper::DisplayVesperInfo_ImGui();
	ImGui::End();
}

void Sandbox2D::OnEvent(Vesper::Event& e)
{
	m_CameraController.OnEvent(e);
}
