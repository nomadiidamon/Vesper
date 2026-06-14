#pragma once

#include <Vesper.h>

#include "Vesper/App/Layer.h"
#include "Vesper/ParticleSystem/ParticleSystem.h"
#include "Vesper/Scene/Scene.h"
#include "../Panels/SceneHierarchyPanel.h"
#include "Vesper/Renderer/EditorCamera.h"
#include "ViewportLayer.h"

namespace Vesper {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
		void ResetScene();
	private:
		SceneHierarchyPanel m_SceneHierarchyPanel;
		
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
		SceneState m_SceneState = SceneState::Edit;

		int m_GizmoType = -1;
		float m_TranslationSnap = 0.5f, m_RotationSnap = 45.0f, m_ScaleSnap = 0.5f;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0,0};
		glm::vec2 m_ViewportBounds[2] = { {0,0}, {0,0} };

		
		OrthographicCameraController m_CameraController;

		float lastFrameTime = 0.0f;
		//Entity m_FireEntity, m_SmokeEntity;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Texture2D> m_CheckerboardTexture;

		Ref<ViewportLayer> m_ViewportLayer;
		Ref<Framebuffer> m_Framebuffer;

		Ref<ViewportLayer> m_SecondaryViewportLayer;
		Ref<Framebuffer> m_SecondaryFramebuffer;

		EditorCamera m_EditorCamera;

		//int m_ParticleEmitCount = 100;
		ParticleSystem m_ParticleSystem;	
		ParticleProps m_ParticleProps;

		glm::vec4 m_ClearColor = { 0.1f, 0.3f, 0.3f, 1.0f };

		std::unordered_map<char, Ref<SubTexture2D>>s_TextureMap;

	};


}