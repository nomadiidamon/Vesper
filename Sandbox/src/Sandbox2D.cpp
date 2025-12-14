#include <Vesper/ImGui/VesperImGui.h>

#include "Sandbox2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct RandomProperties {
	glm::vec3 pos;
	glm::vec2 size;
	glm::vec4 color;
	float rotation;
};


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

	Vesper::Renderer2D::ResetStats();

	{
		VZ_PROFILE_SCOPE("Renderer Prep");
		// Render
		Vesper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vesper::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;
		VZ_PROFILE_SCOPE("Renderer2D Draw");

		{
			VZ_PROFILE_SCOPE("Scene 1");
			Vesper::Renderer2D::BeginScene(m_CameraController.GetCamera());

			// Checkerboard background
			Vesper::Renderer2D::DrawQuadWithTexture({ 0.0f, 0.0f, -0.25f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f, m_BackgroundColor);

			// Squares
			Vesper::Renderer2D::DrawQuadRotated({ 0.0f, 1.25f, -0.2f }, { 1.0f, 1.0f }, glm::radians(45.0f + m_squareRotation + rotation), m_SquareColor);

			// Rotated Squares
			if (m_UseSpecialQuadColor)
				Vesper::Renderer2D::DrawQuadRotatedWithTexture({ 0.0f, 1.25f, -0.15f }, { 0.75f, 0.75f }, m_CheckerboardTexture, glm::radians(m_squareRotation * m_specialQuadRotation * rotation), m_textureScale, { 0.90f, 0.85f, 0.2f, 1.0f });
			else
				Vesper::Renderer2D::DrawQuadRotatedWithTexture({ 0.0f, 1.25f, -0.15f }, { 0.75f, 0.75f }, m_CheckerboardTexture, glm::radians(m_squareRotation * m_specialQuadRotation * rotation), m_textureScale, m_SpecialQuadColor);

			Vesper::Renderer2D::DrawQuadRotatedWithTexture({ 2.0f, -0.25f, -0.15f }, { 1.0f, 1.0f }, m_CheckerboardTexture, glm::radians(m_squareRotation + rotation), m_textureScale, m_TextureTintColor1);
			Vesper::Renderer2D::DrawQuadRotatedWithTexture({ -2.0f, -0.25f, -0.15f }, { 1.0f, 1.0f }, m_CheckerboardTexture, glm::radians(m_squareRotation + rotation), m_textureScale, m_TextureTintColor2);

			for (int y = -5; y < 5; y++)
			{
				for (int x = -5; x < 5; x++)
				{
					glm::vec3 pos = glm::vec3(x * 0.15f, y * 0.15f, 0.0f);
					Vesper::Renderer2D::DrawQuad(pos, { 2.5f, 2.5f }, { (x + 5) / 10.0f, 0.4f, (y + 5) / 10.0f, 0.55f });
				}
			}
			Vesper::Renderer2D::EndScene();

		}

		{
			VZ_PROFILE_SCOPE("Scene 2");
			Vesper::Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f,(y + 5.0f) / 10.0f, 0.35f };
					Vesper::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}
			Vesper::Renderer2D::EndScene();

		}
	}

}

void Sandbox2D::OnImGuiRender()
{
	VZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	ImGui::Text("Renderer2D Stats:");

	auto stats = Vesper::Renderer2D::GetStats();
	ImGui::Text("\tDraw Calls: %d", stats.DrawCalls);
	ImGui::Text("\tQuad Count: %d", stats.QuadCount);
	ImGui::Text("\tVertex Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("\tIndex Count: %d", stats.GetTotalIndexCount());


	if (ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor)))
	{
	}

	if (ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor)))
	{

	}

	if (ImGui::Checkbox("Use Special Quad Color", &m_UseSpecialQuadColor))
	{
	}

	if (ImGui::ColorEdit4("Special Quad Color", glm::value_ptr(m_SpecialQuadColor)))
	{
	}

	if (ImGui::ColorEdit4("Texture Tint Color 1", glm::value_ptr(m_TextureTintColor1)))
	{
	}
	if (ImGui::ColorEdit4("Texture Tint Color 2", glm::value_ptr(m_TextureTintColor2)))
	{
	}

	if (ImGui::SliderFloat("Texture Scale", &m_textureScale, 0.1f, 10.0f))
	{
	}

	if (ImGui::SliderFloat("Square Rotation", &m_squareRotation, 0.0f, 360.0f))
	{
	}
	if (ImGui::SliderFloat("Special Quad Rotation", &m_specialQuadRotation, 0.1f, 5.0f))
	{
	}
	Vesper::DisplayVesperInfo_ImGui();
	ImGui::End();
}

void Sandbox2D::OnEvent(Vesper::Event& e)
{
	m_CameraController.OnEvent(e);
}
