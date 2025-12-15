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
	Vesper::Ref<Vesper::SubTexture2D> m_SubTexture1;
	Vesper::Ref<Vesper::SubTexture2D> m_SubTexture2;

	float m_textureScale = 1.0f;
	float m_squareRotation = 25.0f;
	float m_specialQuadRotation = 0.5f;
	int ParticleEmitCount = 100;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_TextureTintColor1 = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_TextureTintColor2 = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec4 m_SpecialQuadColor = { 0.9f, 0.2f, 0.8f, 1.0f };
	bool m_UseSpecialQuadColor = false;

	ParticleSystem m_ParticleSystem;
	ParticleProps m_ParticleProps;

};

