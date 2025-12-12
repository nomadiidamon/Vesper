#include "vzpch.h"
#include "OrthographicCameraController.h"

#include "Vesper/Input.h"
#include "Vesper/KeyCodes.h"

/// Temporary
#include <imgui.h>


namespace Vesper {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Rotation(rotation),
		camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(VZ_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(VZ_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Input::IsKeyPressed(VZ_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Input::IsKeyPressed(VZ_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(VZ_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(VZ_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			camera.SetRotation(m_CameraRotation);
		}
		camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(VZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(VZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	void OrthographicCameraController::SetPosition(float x, float y)
	{
		m_CameraPosition = { x, y, 0.0f };
		camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::SetMoveSpeed(float speed)
	{
		m_CameraMoveSpeed = speed;
	}

	void OrthographicCameraController::SetRotation(float rotation)
	{
		m_CameraRotation = rotation;
		camera.SetRotation(m_CameraRotation);
	}

	void OrthographicCameraController::SetRotationSpeed(float speed)
	{
		m_CameraRotationSpeed = speed;
	}

	float OrthographicCameraController::GetAspectRatio() const
	{
		return m_AspectRatio;
	}

	void OrthographicCameraController::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		camera = OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}


	/// TODO: move to an editor component that can be attached to any system (EditorAbstractionComp)
	void OrthographicCameraController::OnImGuiRender()
	{
		//ImGui::Begin("Settings");

		static float camPos[3] = { GetPosition().x, GetPosition().y, GetPosition().z };
		if (ImGui::DragFloat3("Cam Pos", camPos, 0.1f)) {
			SetPosition(camPos[0], camPos[1]);
		}
		static float camMoveSpeed = GetMoveSpeed();
		if (ImGui::DragFloat("Cam Speed", &camMoveSpeed, 0.1f)) {
			SetMoveSpeed(camMoveSpeed);
		}
		ImGui::Separator();
		static bool rotate = CanRotate();
		if (ImGui::Checkbox("Rotate?", &rotate)) {
			SetCanRotate(rotate);
		}
		ImGui::Separator();
		static float camRot = GetRotation();
		if (ImGui::DragFloat("Cam Rotation", &camRot, 0.1f)) {
			SetRotation(camRot);
		}
		static float camRotSpeed = GetRotationSpeed();
		if (ImGui::DragFloat("Cam Rot Speed", &camRotSpeed, 1.0f)) {
			SetRotationSpeed(camRotSpeed);
		}
		//ImGui::End();

	}
}
