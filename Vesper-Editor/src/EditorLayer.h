#pragma once

#include <Vesper.h>

#include "Vesper/Core/Layer.h"
#include "Vesper/ParticleSystem/ParticleSystem.h"

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
		OrthographicCameraController m_CameraController;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0,0};
		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Texture2D> m_CheckerboardTexture;

		Ref<Texture2D> m_SpriteSheetFire;
		Ref<Texture2D> m_SpriteSheetTown;
		Ref<Texture2D> m_SpriteSheetCrystals;
		Ref<Texture2D> m_SpriteSheetRocks;
		Ref<Texture2D> m_SpriteSheetCursedLands;

		Ref<SubTexture2D> m_SubTextureFire;
		Ref<SubTexture2D> m_SubTextureTown;
		//Ref<SubTexture2D> m_SubTextureCrystal;
		//Ref<SubTexture2D> m_SubTextureRock;
		//Ref<SubTexture2D> m_SubTexturePlant;

		Ref<Framebuffer> m_Framebuffer;

		float m_textureScale = 1.0f;
		float m_squareRotation = 25.0f;
		float m_specialQuadRotation = 0.5f;
		int ParticleEmitCount = 100;

		ParticleSystem m_ParticleSystem;	
		ParticleProps m_ParticleProps;


		bool scene1 = true, scene2 = false, scene3 = false, scene4 = false;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		glm::vec4 m_TextureTintColor1 = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4 m_TextureTintColor2 = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		glm::vec4 m_ClearColor = { 0.1f, 0.3f, 0.3f, 1.0f };
		glm::vec4 m_SpecialQuadColor = { 0.9f, 0.2f, 0.8f, 1.0f };
		bool m_UseSpecialQuadColor = false;

		std::unordered_map<char, Ref<SubTexture2D>>s_TextureMap;

	};


}