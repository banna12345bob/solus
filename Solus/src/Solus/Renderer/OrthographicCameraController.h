#pragma once

#include "Solus/Renderer/OrthographicCamera.h"
#include "Solus/Core/Timestep.h"

#include "Solus/Events/AllEvents.h"

namespace Solus {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectratio, glm::vec3 position = {0.0f, 0.0f, 0.0f}, bool rotation = false, bool movement = true, bool canZoom = true);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CaculateView(); }

		const OrthographicCameraBounds& getBounds() const { return m_Bounds; }
		const glm::vec3& getPosition() const { return m_CameraPosition; }
	private:
		void CaculateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowRezise(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		OrthographicCameraBounds m_Bounds;

		bool m_Rotation, m_Movement, m_CanZoom;

		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition;
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 90.0f;
	};

}