#include <Vesper/ImGui/VesperImGui.h>

#include "Sandbox2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 20;
static const uint32_t s_MapHeight = 10;
static const char* s_MapTiles =
"GGGGGGGGGGGGGGGGGGGG"
"GGGCCCCCCCCCCCCCCGGG"
"GGGCGGGGGGGGGGGGCGGG"
"GGGCGGRGGGGGGRGGCGGG"
"GGGCGGGGGGGGGGGGCGGG"
"GGGCGGGGGGGGGGGGCGGG"
"GGGCGGPGGGGGGPGGCGGG"
"GGGCGGGGGGGGGGGGCGGG"
"GGGCCCCCCCCCCCCCCGGG"
"GGGGGGGGGGGGGGGGGGGG";

// G - Grass - SpriteSheetTown -> subTexture(4.25, 0.75) (64, 64) (1, 1)
// C - Crystal - SpriteSheetCrystals -> subTexture { 0, 1.25 }, { 64, 64 }, { 1, 1 }
// R - Rock - SpriteSheetRocks -> subTexture { 0, 3.75 }, { 64, 64 }, { 1, 1 }
// P - Plant - SpriteSheetCursedLands -> subTexture { 0, 1.875 }, { 128, 128 }, { 1, 1 }



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
	m_SpriteSheetFire = Vesper::Texture2D::Create("assets/textures/sheets/fire_01.png");
	m_SpriteSheetTown = Vesper::Texture2D::Create("assets/textures/sheets/town_tilesheet.png");
	m_SpriteSheetCrystals = Vesper::Texture2D::Create("assets/textures/sheets/craftpix/Crystals/Crystals.png");
	m_SpriteSheetRocks = Vesper::Texture2D::Create("assets/textures/sheets/craftpix/Rocks/Rocks_source.png");
	m_SpriteSheetCursedLands = Vesper::Texture2D::Create("assets/textures/sheets/craftpix/CursedLand/Tiled_files/Objects.png");

	m_SubTextureFire = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetFire, { 1, 0 }, { 128, 127 });
	m_SubTextureTown = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetTown, { 4.25, 0.75 }, { 64, 64 }, { 1, 1 });
	//m_SubTextureCrystal = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetCrystals, { 0, 0 }, { 64, 64 }, { 1, 1});
	//m_SubTextureRock = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetRocks, { 0, 0 }, { 64, 64 }, { 1, 1 });
	//m_SubTexturePlant = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetCursedLands, { 2, 0 }, { 64, 64 }, { 1, 1 });

	s_TextureMap['F'] = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetFire, { 1, 0 }, { 128, 127 });
	s_TextureMap['G'] = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetTown, { 4.25, 0.75 }, { 64, 64 }, { 1, 1 });
	s_TextureMap['C'] = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetCrystals, { 0, 1.25 }, { 64, 64 }, { 1, 1 });
	s_TextureMap['R'] = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetRocks, { 0, 3.75 }, { 64, 64 }, { 1, 1 });
	s_TextureMap['P'] = Vesper::SubTexture2D::CreateFromCoords(m_SpriteSheetCursedLands, { 0, 1.875 }, { 128, 128 }, { 1, 1 });

	m_ParticleProps.Position = { 0.0f, 0.0f, 0.0f };
	m_ParticleProps.Velocity = { 0.0f, 0.0f, 0.0f };
	m_ParticleProps.VelocityVariation = { 1.0f, 1.0f, 0.0f };
	m_ParticleProps.ColorBegin = { 1.0f, 0.5f, 0.2f, 1.0f };
	m_ParticleProps.ColorEnd = { 0.2f, 0.3f, 0.8f, 1.0f };
	m_ParticleProps.SizeBegin = 0.5f;
	m_ParticleProps.SizeEnd = 0.0f;
	m_ParticleProps.LifeTime = 3.0f;
	m_ParticleProps.Rotation = 0.0f;
	m_ParticleProps.RotationVariation = 27.0f;

	m_ParticleSystem = ParticleSystem(10000);
	m_ParticleSystem.SetParticleProps(m_ParticleProps);

	m_CameraController.SetZoomLevel(3.5f);

	Vesper::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;

	m_Framebuffer = Vesper::Framebuffer::Create(fbSpec);
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

	// Render
	Vesper::Renderer2D::ResetStats();
	{
		VZ_PROFILE_SCOPE("Renderer Prep");
		m_Framebuffer->Bind();
		Vesper::RenderCommand::SetClearColor(m_ClearColor);
		Vesper::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;
		VZ_PROFILE_SCOPE("Renderer2D Draw");

		if (scene1)
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

		if (scene2)
		{
			VZ_PROFILE_SCOPE("Scene 2");

			Vesper::Renderer2D::BeginScene(m_CameraController.GetCamera());

			// Sprite sheet drawn as full texture
			Vesper::Renderer2D::DrawQuadWithTexture({ -1.0f, 1.5f, 0.5f }, { 1, 1 }, m_SpriteSheetFire, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			Vesper::Renderer2D::DrawQuadRotatedWithTexture({ 1.5f, 0.0f, 0.0f }, { 1.78f, 1.0f }, m_SpriteSheetTown, 0, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });


			// Sprite sheet drawn as full texture rotated
			Vesper::Renderer2D::DrawQuadRotatedWithTexture({ -1.5f, 0.0f, 0.0f }, { 1.78f, 1.0f }, m_SpriteSheetTown, glm::radians(-rotation), 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

			// Sub texture from tilesheet
			Vesper::Renderer2D::DrawQuadWithTexture({ 2.0f, -1.5f, 0.0f }, { 1.0f, 1.0f }, m_SubTextureTown, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

			// Grid of sub textures from tilesheet
			for (int y = -5; y < 5; y++)
			{
				for (int x = -5; x < 5; x++)
				{
					glm::vec3 pos = glm::vec3(x * 0.09f, y * 0.09f, -0.09f);
					Vesper::Renderer2D::DrawQuadWithTexture(pos, { 0.1f, 0.1f }, m_SubTextureTown, 1.0f, glm::vec4(1.0f));
				}
			}

			/// TODO: get it to animate through texture sheet sub texture indices
			Vesper::Renderer2D::DrawQuadRotatedWithTexture({ 0.0f, -1.5f, 0.0f }, { 1.0f, 1.0f }, m_SubTextureFire, 0, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

			Vesper::Renderer2D::EndScene();
		}

		if (scene3)
		{
			VZ_PROFILE_SCOPE("Scene 3");
			Vesper::Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (uint32_t y = 0; y < s_MapHeight; y++)
			{
				for (uint32_t x = 0; x < s_MapWidth; x++)
				{
					char tileChar = s_MapTiles[x + y * s_MapWidth];
					Vesper::Ref<Vesper::SubTexture2D> texture;
					if (s_TextureMap.find(tileChar) != s_TextureMap.end())
						texture = s_TextureMap[tileChar];
					else
						texture = s_TextureMap['G']; // Default to grass

					Vesper::Renderer2D::DrawQuadWithTexture({ x - s_MapWidth / 2.0f, s_MapHeight - y - s_MapHeight / 2.0f, 0.1f }, { 1.0f, 1.0f }, texture, 1.0f, glm::vec4(1.0f));

				}
			}
			Vesper::Renderer2D::EndScene();
		}

		if (scene4)
		{
			VZ_PROFILE_SCOPE("Scene 4");
			Vesper::Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f,(y + 5.0f) / 10.0f, 0.35f };
					Vesper::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}

			if (Vesper::Input::IsMouseButtonPressed(VZ_MOUSE_BUTTON_LEFT))
			{
				auto [x, y] = Vesper::Input::GetMousePosition();
				auto width = Vesper::Application::Get().GetWindow().GetWidth();
				auto height = Vesper::Application::Get().GetWindow().GetHeight();

				auto bounds = m_CameraController.GetBounds();
				auto pos = m_CameraController.GetPosition();
				m_ParticleProps.Position.x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f + pos.x;
				m_ParticleProps.Position.y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight() + pos.y;
				for (int i = 0; i < ParticleEmitCount; i++) {
					m_ParticleSystem.Emit(m_ParticleProps);
				}
			}
			m_ParticleSystem.OnUpdate(ts);
			m_ParticleSystem.OnRender(m_CameraController.GetCamera());
			Vesper::Renderer2D::EndScene();

		}
	}

	m_Framebuffer->Unbind();
}

void Sandbox2D::OnImGuiRender()
{
	VZ_PROFILE_FUNCTION();

	static bool dockingEnabled = true;
	if (dockingEnabled)
	{


		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.


				if (ImGui::MenuItem("Exit"))
					Vesper::Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		{
			if (ImGui::Begin("Scenes"))
			{
				if (ImGui::Checkbox("Scene 1 - Basic Shapes", &scene1)) {
					if (scene1) {
						scene2 = false;
						scene3 = false;
						scene4 = false;
					}
				}
				if (ImGui::Checkbox("Scene 2 - Sprite Sheets", &scene2)) {
					if (scene2) {
						scene1 = false;
						scene3 = false;
						scene4 = false;
					}
				}
				if (ImGui::Checkbox("Scene 3 - Tile Map", &scene3)) {
					if (scene3) {
						scene1 = false;
						scene2 = false;
						scene4 = false;
					}
				}
				if (ImGui::Checkbox("Scene 4 - Particle System", &scene4)) {
					if (scene4) {
						scene1 = false;
						scene2 = false;
						scene3 = false;
					}
				}
			}
			ImGui::End();
		}


		{
			ImGui::Begin("Settings");
			ImGui::Text("Renderer2D Stats:");
			auto stats = Vesper::Renderer2D::GetStats();
			ImGui::Text("\tDraw Calls: %d", stats.DrawCalls);
			ImGui::Text("\tQuad Count: %d", stats.QuadCount);
			ImGui::Text("\tVertex Count: %d", stats.GetTotalVertexCount());
			ImGui::Text("\tIndex Count: %d", stats.GetTotalIndexCount());

			if (ImGui::ColorEdit4("Background Color", glm::value_ptr(m_ClearColor)))
			{
				Vesper::RenderCommand::SetClearColor(m_ClearColor);
			}
			ImGui::End();
		}

		Vesper::DisplayVesperInfo_ImGui();
		{
			ImGui::Begin("Viewport");
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image(textureID, ImVec2(1280, 720), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
		}
	}

	ImGui::End();
}

void Sandbox2D::OnEvent(Vesper::Event& e)
{
	m_CameraController.OnEvent(e);
}
