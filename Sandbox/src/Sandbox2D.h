#pragma once

#include <Vesper.h>

#include "Vesper/Core/Layer.h"
#include "ParticleSystem/ParticleSystem.h"

struct RandomProperties;

class Sandbox2D : public Vesper::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Vesper::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vesper::Event& e) override;
private:
	Vesper::OrthographicCameraController m_CameraController;
	// Temp
	Vesper::Ref<Vesper::VertexArray> m_SquareVA;
	Vesper::Ref<Vesper::Shader> m_FlatColorShader;
	Vesper::Ref<Vesper::Texture2D> m_CheckerboardTexture;

	Vesper::Ref<Vesper::Texture2D> m_SpriteSheetFire;
	Vesper::Ref<Vesper::Texture2D> m_SpriteSheetTown;
	Vesper::Ref<Vesper::Texture2D> m_SpriteSheetCrystals;
	Vesper::Ref<Vesper::Texture2D> m_SpriteSheetRocks;
	Vesper::Ref<Vesper::Texture2D> m_SpriteSheetCursedLands;

	Vesper::Ref<Vesper::SubTexture2D> m_SubTextureFire;
	Vesper::Ref<Vesper::SubTexture2D> m_SubTextureTown;
	//Vesper::Ref<Vesper::SubTexture2D> m_SubTextureCrystal;
	//Vesper::Ref<Vesper::SubTexture2D> m_SubTextureRock;
	//Vesper::Ref<Vesper::SubTexture2D> m_SubTexturePlant;

	Vesper::Ref<Vesper::Framebuffer> m_Framebuffer;

	float m_textureScale = 1.0f;
	float m_squareRotation = 25.0f;
	float m_specialQuadRotation = 0.5f;
	int ParticleEmitCount = 100;
	
	bool scene1 = false, scene2 = false, scene3 = false, scene4 = true;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_TextureTintColor1 = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_TextureTintColor2 = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec4 m_ClearColor = { 0.1f, 0.3f, 0.3f, 1.0f };
	glm::vec4 m_SpecialQuadColor = { 0.9f, 0.2f, 0.8f, 1.0f };
	bool m_UseSpecialQuadColor = false;

	ParticleSystem m_ParticleSystem;
	ParticleProps m_ParticleProps;

	std::unordered_map<char, Vesper::Ref<Vesper::SubTexture2D>>s_TextureMap;

};

