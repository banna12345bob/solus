#include "supch.h"
#include "Solus/Renderer/OrthographicCameraController.h"

#include "Solus/Core/Input.h"
#include "Solus/Core/KeyCodes.h"

namespace Solus {

	OrthographicCameraController::OrthographicCameraController(float aspectratio, glm::vec3 position, bool rotation, bool movement, bool canZoom)
		: m_AspectRatio(aspectratio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_CameraPosition(-position), m_Movement(movement), m_CanZoom(canZoom), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		SU_PROFILE_FUNCTION();
		if (m_Movement)
		{
			if (Input::IsKeyPressed(SU_KEY_D))
			{
				m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
				m_Bounds = { -m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, -m_ZoomLevel - m_CameraPosition.y, m_ZoomLevel - m_CameraPosition.y };
			}

			else if (Input::IsKeyPressed(SU_KEY_A))
			{
				m_CameraPosition.x += m_CameraTranslationSpeed * ts;
				m_Bounds = { -m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, -m_ZoomLevel - m_CameraPosition.y, m_ZoomLevel - m_CameraPosition.y };
			}


			if (Input::IsKeyPressed(SU_KEY_S))
			{
				m_CameraPosition.y += m_CameraTranslationSpeed * ts;
				m_Bounds = { -m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, -m_ZoomLevel - m_CameraPosition.y, m_ZoomLevel - m_CameraPosition.y };
			}


			else if (Input::IsKeyPressed(SU_KEY_W))
			{
				m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
				m_Bounds = { -m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, -m_ZoomLevel - m_CameraPosition.y, m_ZoomLevel - m_CameraPosition.y };
			}
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(SU_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(SU_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		SU_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SU_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SU_BIND_EVENT_FN(OrthographicCameraController::OnWindowRezise));
	}

	void OrthographicCameraController::CaculateView()
	{
		//m_Bounds = { -m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, m_AspectRatio * m_ZoomLevel - m_CameraPosition.x, -m_ZoomLevel - m_CameraPosition.y, m_ZoomLevel - m_CameraPosition.y };
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		SU_PROFILE_FUNCTION();
		if (m_CanZoom)
		{
			m_ZoomLevel -= e.GetYOffset();
			m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
			CaculateView();
			return false;
		}
		return false;
	}

	bool OrthographicCameraController::OnWindowRezise(WindowResizeEvent& e)
	{
		SU_PROFILE_FUNCTION();
		if (m_CanZoom)
		{
			m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
			CaculateView();
			return false;
		}
		return false;
	}

}