#include <Vesper/GUI/ImGui/VesperImGui.h>
#include <ImGuizmo.h>
#include <Vesper/Utils/PlatformUtils.h>

#include "Vesper/Core/Math.h"
#include "EditorLayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vesper/Scene/SceneSerializer.h"
#include "ViewportLayer.h"

#include "Vesper/App/ResourceManager.h"


namespace Vesper {


	EditorLayer::EditorLayer()
		: Layer("Editor Layer"), m_CameraController(2160.0f / 1400.0f, true)
	{
		VZ_PROFILE_FUNCTION();
	}



	void EditorLayer::OnAttach()
	{
		VZ_PROFILE_FUNCTION();

		// Texture / SubTexture setup
		{
			/// TODO: Move predefined textures to resource manager
			/// TODO: Fix pathing to use the resource manager utils
			m_CheckerboardTexture = Texture2D::Create("Cavernae/assets/textures/Checkerboard.png");
			
			/*
			m_SpriteSheetFire = Texture2D::Create("../../Cavernae/assets/textures/sheets/fire_01.png");
			m_SpriteSheetSmoke = Texture2D::Create("../../Cavernae/assets/textures/sheets/fire_02.png");
			m_SpriteSheetTown = Texture2D::Create("../../Cavernae/assets/textures/sheets/town_tilesheet.png");
			m_SpriteSheetCrystals = Texture2D::Create("../../Cavernae/assets/textures/sheets/craftpix/Crystals/Crystals.png");
			m_SpriteSheetRocks = Texture2D::Create("../../Cavernae/assets/textures/sheets/craftpix/Rocks/Rocks_source.png");
			m_SpriteSheetCursedLands = Texture2D::Create("../../Cavernae/assets/textures/sheets/craftpix/CursedLand/Tiled_files/Objects.png");

			m_SubTextureFire = SubTexture2D::CreateFromCoords(m_SpriteSheetFire, { 1, 0 }, { 128, 127 });
			m_SubTextureSmoke = SubTexture2D::CreateFromCoords(m_SpriteSheetSmoke, { 1, 0 }, { 128, 127 });
			m_SubTextureTown = SubTexture2D::CreateFromCoords(m_SpriteSheetTown, { 4.25, 0.75 }, { 64, 64 }, { 1, 1 });
			s_TextureMap['F'] = SubTexture2D::CreateFromCoords(m_SpriteSheetFire, { 1, 0 }, { 128, 127 });
			s_TextureMap['G'] = SubTexture2D::CreateFromCoords(m_SpriteSheetTown, { 4.25, 0.75 }, { 64, 64 }, { 1, 1 });
			s_TextureMap['C'] = SubTexture2D::CreateFromCoords(m_SpriteSheetCrystals, { 0, 1.25 }, { 64, 64 }, { 1, 1 });
			s_TextureMap['R'] = SubTexture2D::CreateFromCoords(m_SpriteSheetRocks, { 0, 3.75 }, { 64, 64 }, { 1, 1 });
			s_TextureMap['P'] = SubTexture2D::CreateFromCoords(m_SpriteSheetCursedLands, { 0, 1.875 }, { 128, 128 }, { 1, 1 });
			*/
		
		}


		// Particle setup
		{
			m_ParticleProps.Position = { 0.0f, 0.0f, 0.0f };
			m_ParticleProps.Velocity = { 0.0f, 0.0f, 0.0f };
			m_ParticleProps.VelocityVariation = { 1.0f, 1.0f, 0.0f };
			m_ParticleProps.ColorBegin = { 1.0f, 0.5f, 0.2f, 1.0f };
			m_ParticleProps.ColorEnd = { 0.2f, 0.3f, 0.8f, 1.0f };
			m_ParticleProps.SizeBegin = { 0.5f, 0.5f };
			m_ParticleProps.SizeEnd = { 0.0f, 0.0f };
			m_ParticleProps.Lifetime = 3.0f;
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
			m_ActiveScene = CreateRef<Scene>();

			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);

			if (VZ_EDITOR_USE_DEFAULT_SCENE) {
				/// TODO: Get an automatic path to resource that is NOT hardcoded
				std::string sceneName = VZ_EDITOR_DEFAULT_SCENE;
				std::filesystem::path sceneDirectory = Vesper::ResourceManager::JoinPaths(Vesper::Application::Get().GetResourceManager().GetResourcesDirectory(), "Scenes");
				std::filesystem::path scenePath = Vesper::ResourceManager::JoinPaths(sceneDirectory.string(), sceneName);
				std::filesystem::path loadedScene = Vesper::ResourceManager::NormalizePath(scenePath.string());

				bool valid = serializer.Deserialize(loadedScene.string());
				if (!valid) {
					VZ_CORE_ERROR("Failed to load default scene: " + loadedScene.string());
					VZ_CORE_ERROR("Attempted Scene: " + std::string(scenePath.string()));
					VZ_CORE_ERROR("Current Working Directory: " + Vesper::Application::Get().GetResourceManager().GetWorkingDirectory());
					VZ_CORE_ERROR("Absolute Path Attempted: " + loadedScene.string());
					VZ_CORE_ERROR("Error loading the scene, please check the paths and file availability.");
				}
				else {
					VZ_CORE_INFO("Successfully loaded the default scene: " + loadedScene.string());
				}
			}
			m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		}

		// create and wire the viewport layer
		m_ViewportLayer = CreateRef<Vesper::ViewportLayer>("Viewport");
		m_ViewportLayer->SetFramebuffer(m_Framebuffer);

		m_ViewportLayer->SetOnResizeCallback([this](uint32_t w, uint32_t h) {
			m_ViewportSize = { (float)w, (float)h };
			m_CameraController.OnResize((float)w, (float)h);
			m_EditorCamera.SetViewportSize((float)w, (float)h);
			if (m_ActiveScene)
				m_ActiveScene->OnViewportResize(w, h);
			});
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
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
		}
		m_EditorCamera.OnUpdate(ts);

		Renderer2D::ResetStats();
		{
			VZ_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor(m_ClearColor);
			RenderCommand::Clear();
		}
		{
			VZ_PROFILE_SCOPE("Entity Scene Update");
			// Update scene
			m_ActiveScene->OnUpdateRuntime(ts);
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
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

			// set up in-window render callback for gizmos
			m_ViewportLayer->SetInsideRenderCallback([this]() {
				// Gizmos
				Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
				if (selectedEntity && m_GizmoType != -1)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();
					float windowWidth = (float)ImGui::GetWindowWidth();
					float windowHeight = (float)ImGui::GetWindowHeight();
					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

					// Editor camera
					const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
					glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

					// Entity Transform
					auto& tc = selectedEntity.GetComponent<TransformComponent>();
					glm::mat4 transform = tc.GetTransform();

					bool snap = Input::IsKeyPressed(Key::LeftControl);
					float snapValue = m_TranslationSnap;
					if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
						snapValue = m_RotationSnap;
					else if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
						snapValue = m_ScaleSnap;

					float snapValues[3] = { snapValue, snapValue, snapValue };

					ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
						(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
						nullptr, snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing())
					{
						glm::vec3 translation, rotation, scale;
						Vesper::Math::DecomposeTransform(transform, translation, rotation, scale);
						tc.Translation = translation;
						tc.Rotation = rotation;
						tc.Scale = scale;
					}
				}
			});

			// render the viewport window
			m_ViewportLayer->OnImGuiRender();

			// sync focus/hover state and block events for ImGui layer as before
			m_ViewportFocused = m_ViewportLayer->IsFocused();
			m_ViewportHovered = m_ViewportLayer->IsHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

			// update stored bounds if you rely on them elsewhere:
			const glm::vec2* vb = m_ViewportLayer->GetViewportBounds();
			m_ViewportBounds[0] = vb[0];
			m_ViewportBounds[1] = vb[1];


			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		if (m_SceneState == SceneState::Edit) {
			m_EditorCamera.OnEvent(e);
		}

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(VZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));

	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			// Scene Shortcuts
		case Key::N:

			if (control)
			{
				NewScene();
			}
			break;

		case Key::O:
			if (control)
			{
				OpenScene();
			}
			break;

		case Key::S:
			if (control && shift)
			{
				SaveSceneAs();
			}
			break;


			// Gizmo Shortcuts
		case Key::Q:
			m_GizmoType = -1;
			break;
		case Key::W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
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