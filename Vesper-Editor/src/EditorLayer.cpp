#include <Vesper/ImGui/VesperImGui.h>
#include <Vesper/Utils/PlatformUtils.h>

#include "EditorLayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vesper/Scene/SceneSerializer.h"

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


namespace Vesper {


	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
	{
		VZ_PROFILE_FUNCTION();
	}



	void EditorLayer::OnAttach()
	{
		VZ_PROFILE_FUNCTION();

		// Texture / SubTexture setup
		{
			m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
			m_SpriteSheetFire = Texture2D::Create("assets/textures/sheets/fire_01.png");
			m_SpriteSheetSmoke = Texture2D::Create("assets/textures/sheets/fire_02.png");
			m_SpriteSheetTown = Texture2D::Create("assets/textures/sheets/town_tilesheet.png");
			m_SpriteSheetCrystals = Texture2D::Create("assets/textures/sheets/craftpix/Crystals/Crystals.png");
			m_SpriteSheetRocks = Texture2D::Create("assets/textures/sheets/craftpix/Rocks/Rocks_source.png");
			m_SpriteSheetCursedLands = Texture2D::Create("assets/textures/sheets/craftpix/CursedLand/Tiled_files/Objects.png");

			m_SubTextureFire = SubTexture2D::CreateFromCoords(m_SpriteSheetFire, { 1, 0 }, { 128, 127 });
			m_SubTextureSmoke = SubTexture2D::CreateFromCoords(m_SpriteSheetSmoke, { 1, 0 }, { 128, 127 });
			m_SubTextureTown = SubTexture2D::CreateFromCoords(m_SpriteSheetTown, { 4.25, 0.75 }, { 64, 64 }, { 1, 1 });
			s_TextureMap['F'] = SubTexture2D::CreateFromCoords(m_SpriteSheetFire, { 1, 0 }, { 128, 127 });
			s_TextureMap['G'] = SubTexture2D::CreateFromCoords(m_SpriteSheetTown, { 4.25, 0.75 }, { 64, 64 }, { 1, 1 });
			s_TextureMap['C'] = SubTexture2D::CreateFromCoords(m_SpriteSheetCrystals, { 0, 1.25 }, { 64, 64 }, { 1, 1 });
			s_TextureMap['R'] = SubTexture2D::CreateFromCoords(m_SpriteSheetRocks, { 0, 3.75 }, { 64, 64 }, { 1, 1 });
			s_TextureMap['P'] = SubTexture2D::CreateFromCoords(m_SpriteSheetCursedLands, { 0, 1.875 }, { 128, 128 }, { 1, 1 });
		}


		// Particle setup
		{
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
		}

		// Framebuffer/Viewport setup
		{
			FramebufferSpecification fbSpec;
			fbSpec.Width = 1280;
			fbSpec.Height = 720;
			m_Framebuffer = Framebuffer::Create(fbSpec);
		}

		// Scene setup
		{
			m_CameraController.SetZoomLevel(5.5f);
			m_ActiveScene = CreateRef<Scene>();

#if 0

			m_CameraEntity = m_ActiveScene->CreateEntity("Primary Camera Entity");
			auto& pCam = m_CameraEntity.AddComponent<CameraComponent>();
			pCam.Primary = true;
			pCam.Camera.SetPerspective(glm::radians(45.0f), 0.1f, 1000.0f);
			auto& pos = m_CameraEntity.GetComponent<TransformComponent>().Translation;
			pos.x += 1.25f;
			pos.z += 5.0f;

			m_SecondaryCameraEntity = m_ActiveScene->CreateEntity("Secondary Camera Entity");
			auto& cc = m_SecondaryCameraEntity.AddComponent<CameraComponent>().Primary = false;
			m_SecondaryCameraEntity.GetComponent<TransformComponent>();

			auto fbSpec = m_Framebuffer->GetSpecification();
			m_ActiveScene->OnViewportResize(fbSpec.Width, fbSpec.Height);

			// Animation 1
			{
				auto square = m_ActiveScene->CreateEntity("Fire Animation");
				auto& transform = square.GetComponent<TransformComponent>();
				transform.Translation = (glm::vec3(-0.5f, 0.0f, -1.5f));

				square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.8f, 0.2f, 1.0f });
				std::vector<Ref<SubTexture2D>> fireFrames;
				for (int x = 0; x < 63; x++)
				{
					for (int y = 0; y < 2; y++)
					{
						fireFrames.push_back(SubTexture2D::CreateFromCoords(m_SpriteSheetFire, { (float)y, (float)x }, { 128, 128 }));
					}
				}
				TextureAnimationComponent texAnim(fireFrames, 0.05f);
				square.AddComponent<TextureAnimationComponent>(texAnim);
				m_FireEntity = square;
			}

			// Animation 2
			{
				auto square = m_ActiveScene->CreateEntity("Smoke Animation");
				auto& transform = square.GetComponent<TransformComponent>();
				// adjust the position of the square entity
				transform.Translation = (glm::vec3(0.5f, 0.0f, 1.5f));

				square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.8f, 0.8f, 0.2f, 1.0f });
				std::vector<Ref<SubTexture2D>> smokeFrames;
				for (int x = 0; x < 63; x++)
				{
					for (int y = 0; y < 2; y++)
					{
						smokeFrames.push_back(SubTexture2D::CreateFromCoords(m_SpriteSheetSmoke, { (float)y, (float)x }, { 128, 128 }));
					}

				}
				TextureAnimationComponent texAnim(smokeFrames, 0.05f);
				square.AddComponent<TextureAnimationComponent>(texAnim);
				m_SmokeEntity = square;
			}

			auto quadEntity = m_ActiveScene->CreateEntity("Quad Entity");
			quadEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.3f, 0.8f, 1.0f });
			quadEntity.GetComponent<TransformComponent>().Scale = { 0.5f, 0.5f, 1.0f };
			quadEntity.GetComponent<TransformComponent>().Translation = { 1.5f, 0.0f, 0.0f };


			class CameraController : public ScriptableEntity {
			public:
				void OnCreate()
				{
					GetComponent<TransformComponent>().Translation = (glm::vec3(Random::RangeF1(-3.0f, 3.0f), Random::RangeF1(-3.0f, 3.0f), 0.0f));

				}

				void OnDestroy()
				{

				}

				void OnUpdate(Timestep ts)
				{
					auto& transform = GetComponent<TransformComponent>().GetTransform();
					float speed = 5.0f;


					if (Input::IsKeyPressed(VZ_KEY_A))
						transform[3][0] -= speed * ts;

					if (Input::IsKeyPressed(VZ_KEY_D))
						transform[3][0] += speed * ts;

					if (Input::IsKeyPressed(VZ_KEY_W))
						transform[3][1] += speed * ts;

					if (Input::IsKeyPressed(VZ_KEY_S))
						transform[3][1] -= speed * ts;
				}

			};

			m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
			m_SecondaryCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif

			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize("assets/scenes/Example.vesper");
			VZ_CORE_INFO("Scene serialized to assets/scenes/Example.vesper");
		}


	}

	void EditorLayer::OnDetach()
	{
		VZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		VZ_PROFILE_FUNCTION();

		// Resize
		if (Vesper::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != (uint32_t)m_ViewportSize.x || spec.Height != (uint32_t)m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		{
			VZ_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor(m_ClearColor);
			RenderCommand::Clear();
		}


		{
			static float rotation = 0.0f;
			rotation += ts * 50.0f;
			VZ_PROFILE_SCOPE("Renderer2D Draw");

			if (scene1)
			{
				VZ_PROFILE_SCOPE("Scene 1");
				Renderer2D::BeginScene(m_CameraController.GetCamera());

				// Checkerboard background
				Renderer2D::DrawQuadWithTexture({ 0.0f, 0.0f, -0.25f }, { 25.0f, 25.0f }, m_CheckerboardTexture, 10.0f, m_BackgroundColor);

				// Squares
				Renderer2D::DrawQuadRotated({ 0.0f, 1.25f, -0.165f }, { 1.0f, 1.0f }, glm::radians(45.0f + m_squareRotation + rotation), m_SquareColor);

				// Rotated Squares
				Renderer2D::DrawQuadRotatedWithTexture({ 0.0f, 1.25f, -0.15f }, { 0.75f, 0.75f }, m_CheckerboardTexture, glm::radians(m_squareRotation * m_specialQuadRotation * rotation), m_textureScale, m_SpecialQuadColor);

				Renderer2D::DrawQuadRotatedWithTexture({ 2.0f, -0.25f, -0.15f }, { 1.0f, 1.0f }, m_CheckerboardTexture, glm::radians(m_squareRotation + rotation), m_textureScale, m_TextureTintColor1);
				Renderer2D::DrawQuadRotatedWithTexture({ -2.0f, -0.25f, -0.15f }, { 1.0f, 1.0f }, m_CheckerboardTexture, glm::radians(m_squareRotation + rotation), m_textureScale, m_TextureTintColor2);

				glm::vec3 startPos = { 0.0f, 0.0f, -0.175f };
				//Renderer2D::DrawQuad(pos, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
				glm::vec3 finalPos = startPos;
				float offset = 0.9f;
				for (int y = -10; y <= 10; y++)
				{
					for (int x = -10; x <= 10; x++)
					{
						glm::vec3 newPos = { startPos.x - x * offset, startPos.y - y * offset, startPos.z };
						Renderer2D::DrawQuad(newPos, { 0.8f, 0.8f }, { (x + 5) / 10.0f, 0.4f, (y + 5) / 10.0f, 1.0f });
						finalPos = newPos;
					}
				}
				Renderer2D::DrawQuad(finalPos, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });

				Renderer2D::EndScene();

			}

			if (scene2)
			{
				VZ_PROFILE_SCOPE("Scene 2");

				Renderer2D::BeginScene(m_CameraController.GetCamera());

				// Sprite sheet drawn as full texture
				Renderer2D::DrawQuadWithTexture({ -1.0f, 1.5f, 0.5f }, { 1, 1 }, m_SpriteSheetFire, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
				Renderer2D::DrawQuadRotatedWithTexture({ 1.5f, 0.0f, 0.0f }, { 1.78f, 1.0f }, m_SpriteSheetTown, 0, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });


				// Sprite sheet drawn as full texture rotated
				Renderer2D::DrawQuadRotatedWithTexture({ -1.5f, 0.0f, 0.0f }, { 1.78f, 1.0f }, m_SpriteSheetTown, glm::radians(-rotation), 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

				// Sub texture from tilesheet
				Renderer2D::DrawQuadWithTexture({ 2.0f, -1.5f, 0.0f }, { 1.0f, 1.0f }, m_SubTextureTown, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

				// Grid of sub textures from tilesheet
				for (int y = -5; y < 5; y++)
				{
					for (int x = -5; x < 5; x++)
					{
						glm::vec3 pos = glm::vec3(x * 0.09f, y * 0.09f, -0.09f);
						Renderer2D::DrawQuadWithTexture(pos, { 0.1f, 0.1f }, m_SubTextureTown, 1.0f, glm::vec4(1.0f));
					}
				}

				/// TODO: get it to animate through texture sheet sub texture indices
				Renderer2D::DrawQuadRotatedWithTexture({ 0.0f, -1.5f, 0.0f }, { 1.0f, 1.0f }, m_SubTextureFire, 0, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

				Renderer2D::EndScene();
			}

			if (scene3)
			{
				VZ_PROFILE_SCOPE("Scene 3");
				Renderer2D::BeginScene(m_CameraController.GetCamera());
				for (uint32_t y = 0; y < s_MapHeight; y++)
				{
					for (uint32_t x = 0; x < s_MapWidth; x++)
					{
						char tileChar = s_MapTiles[x + y * s_MapWidth];
						Ref<SubTexture2D> texture;
						if (s_TextureMap.find(tileChar) != s_TextureMap.end())
							texture = s_TextureMap[tileChar];
						else
							texture = s_TextureMap['G']; // Default to grass

						Renderer2D::DrawQuadWithTexture({ x - s_MapWidth / 2.0f, s_MapHeight - y - s_MapHeight / 2.0f, 0.1f }, { 1.0f, 1.0f }, texture, 1.0f, glm::vec4(1.0f));

					}
				}
				Renderer2D::EndScene();
			}

			if (scene4)
			{
				VZ_PROFILE_SCOPE("Scene 4");
				Renderer2D::BeginScene(m_CameraController.GetCamera());
				for (float y = -5.0f; y < 5.0f; y += 0.5f)
				{
					for (float x = -5.0f; x < 5.0f; x += 0.5f)
					{
						glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f,(y + 5.0f) / 10.0f, 0.35f };
						Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
					}
				}

				if (Input::IsMouseButtonPressed(VZ_MOUSE_BUTTON_LEFT) && m_ViewportHovered)
				{
					ImVec2 mousePos = ImGui::GetMousePos();

					mousePos.x -= m_ViewportBounds[0].x;
					mousePos.y -= m_ViewportBounds[0].y;

					if (mousePos.x < 0 || mousePos.y < 0 || mousePos.x > m_ViewportSize.x || mousePos.y > m_ViewportSize.y)
						return;

					auto bounds = m_CameraController.GetBounds();
					auto camPos = m_CameraController.GetPosition();

					float width = m_ViewportSize.x;
					float height = m_ViewportSize.y;

					m_ParticleProps.Position.x = (mousePos.x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f + camPos.x;
					m_ParticleProps.Position.y = bounds.GetHeight() * 0.5f - (mousePos.y / height) * bounds.GetHeight() + camPos.y;

					for (int i = 0; i < ParticleEmitCount; i++) {
						m_ParticleSystem.Emit(m_ParticleProps);
					}
				}
				m_ParticleSystem.OnUpdate(ts);
				m_ParticleSystem.OnRender(m_CameraController.GetCamera());
				Renderer2D::EndScene();

			}

			if (useEntityScene)
			{
				VZ_PROFILE_SCOPE("Entity Scene Update");
				// Update scene
				m_ActiveScene->OnUpdate(ts);
			}

		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		VZ_PROFILE_FUNCTION();

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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSize = style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();
				

				if (ImGui::MenuItem("Open..", "Ctrl+O"))
					OpenScene();
				

				if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))
					SaveSceneAs();
				

				if (ImGui::MenuItem("Reset Scene"))
					ResetScene();
				

				if (ImGui::MenuItem("Exit"))
					Vesper::Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		{
			if (ImGui::Begin("Scenes"))
			{
				if (ImGui::Checkbox("Entity Scene", &useEntityScene)) {
					if (useEntityScene) {
						scene1 = false;
						scene2 = false;
						scene3 = false;
						scene4 = false;
					}
				}
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

		m_SceneHierarchyPanel.OnImGuiRender();

		{
			ImGui::Begin("Settings");
			ImGui::Text("Renderer2D Stats:");
			auto stats = Renderer2D::GetStats();
			ImGui::Text("\tDraw Calls: %d", stats.DrawCalls);
			ImGui::Text("\tQuad Count: %d", stats.QuadCount);
			ImGui::Text("\tVertex Count: %d", stats.GetTotalVertexCount());
			ImGui::Text("\tIndex Count: %d", stats.GetTotalIndexCount());
			ImGui::Text("Application Settings:");
			ImGui::Text("\tFPS: %.1f", ImGui::GetIO().Framerate);
			if (ImGui::ColorEdit4("Background Color", glm::value_ptr(m_ClearColor)))
			{
				RenderCommand::SetClearColor(m_ClearColor);
			}
			ImGui::End();
		}

		DisplayVesperInfo_ImGui();

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("Viewport");
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0.0f && viewportPanelSize.y > 0)
			{
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			}

			ImVec2 viewportBoundsMin = ImGui::GetCursorScreenPos();
			ImVec2 viewportBoundsMax = { viewportBoundsMin.x + m_ViewportSize.x, viewportBoundsMin.y + m_ViewportSize.y };
			m_ViewportBounds[0] = { viewportBoundsMin.x, viewportBoundsMin.y };
			m_ViewportBounds[1] = { viewportBoundsMax.x, viewportBoundsMax.y };

			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image(textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			ImGui::PopStyleVar();
		}
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(VZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));

	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(VZ_KEY_LEFT_CONTROL) || Input::IsKeyPressed(VZ_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(VZ_KEY_LEFT_SHIFT) || Input::IsKeyPressed(VZ_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{

		case VZ_KEY_N:
		
			if (control)
			{
				NewScene();
			}
			break;
		
		case VZ_KEY_O:
			if (control)
			{
				OpenScene();
			}
			break;

		case VZ_KEY_S:
			if (control && shift)
			{
				SaveSceneAs();
			}
			break;
		}
		return false;

	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = FileDialogs::OpenFile("Vesper Scene (*.vesper)\0*.vesper\0");

		if (!filePath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filePath);
			VZ_CORE_INFO("Scene deserialized from: " + filePath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filePath = FileDialogs::SaveFile("Vesper Scene (*.vesper)\0*.vesper\0");
		if (!filePath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filePath);
			VZ_CORE_INFO("Scene serialized to: " + filePath);
		}
	}

	void EditorLayer::ResetScene()
	{
		VZ_CORE_ASSERT(false, "Not implemented yet!");
	}

}