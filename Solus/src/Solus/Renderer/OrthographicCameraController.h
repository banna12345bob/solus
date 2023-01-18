#pragma once

#include "Solus/Renderer/OrthographicCamera.h"
#include "Solus/Core/Timestep.h"

#include "Solus/Events/AllEvents.h"

namespace Solus {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectratio, glm::vec3 position = {0.0f, 0.0f, 0.0f}, bool rotation = false, bool movement = true, bool canZoom = true);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowRezise(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation, m_Movement, m_CanZoom;

		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition;
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 90.0f;
	};

}