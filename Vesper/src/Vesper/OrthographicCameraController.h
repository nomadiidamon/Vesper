#pragma once

#include "Vesper/Renderer/OrthographicCamera.h"
#include "Vesper/Core/Timestep.h"

#include "Vesper/Events/ApplicationEvent.h"
#include "Vesper/Events/MouseEvent.h"



namespace Vesper {

	class OrthographicCameraController
	{
	public: 
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return camera; }
		const OrthographicCamera& GetCamera() const { return camera; }

		glm::vec3 GetPosition() const { return m_CameraPosition; }
		void SetPosition(float x, float y);

		void SetMoveSpeed(float speed);
		float GetMoveSpeed() const { return m_CameraMoveSpeed; }

		void SetRotation(float rotation);
		float GetRotation() const { return m_CameraRotation; }

		void SetRotationSpeed(float speed);
		float GetRotationSpeed() const { return m_CameraRotationSpeed; }

		float GetAspectRatio() const;
		void SetAspectRatio(float aspectRatio);

		bool CanRotate() const { return m_Rotation; }
		void SetCanRotate(bool canRotate) { m_Rotation = canRotate; }

		void OnImGuiRender();
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera camera;
		
		bool m_Rotation = true;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraMoveSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;

	};

}

