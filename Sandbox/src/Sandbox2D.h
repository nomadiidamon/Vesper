#pragma once

#include <Vesper/Vesper.h>

#include "Vesper/Core/Layer.h"

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
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};

