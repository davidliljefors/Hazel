#pragma once

#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/OrthographicCamera.h"


#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera()				{ return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; }
		float GetZoomLevel() const { return m_ZoomLevel; }
		glm::vec3 GetMouseWorldPosition(glm::vec2 mousepos) { return m_Camera.GetMouseWorldPosition(mousepos); }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		float m_AspectRatio { 1.0f };
		float m_ZoomLevel { 1.0f };
		bool m_Rotation;

		glm::vec3 m_CameraPosition { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation { 0.0f };
		float m_CameraMoveSpeed { 1.0f };
		float m_CameraRotationSpeed { 50.0f };

		OrthographicCamera m_Camera;
	};
}