#include "Sandbox2D.h"
#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	SU_PROFILE_FUNCTION();
	m_CheckerboardTexture = Solus::Texture2D::Create("assets/textures/Checkerboard.png");
	Solus::audioPlayer m_AudioPlayer = Solus::audioPlayer();
	m_AudioPlayer.Play("assets/audio/test.wav");
}

void Sandbox2D::OnDetach()
{
	SU_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Solus::Timestep time)
{
	SU_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(time);

	{
		SU_PROFILE_SCOPE("Renderer Prep");

		Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Solus::RenderCommand::Clear();
	}

	{
		SU_PROFILE_SCOPE("Renderer Draw");

		Solus::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Solus::Renderer2D::DrawQuad(glm::vec3(m_CheckerboardPosition[0], m_CheckerboardPosition[1], -0.1f), m_CheckerboardRotation[0], glm::vec2(m_CheckerboardScale[0], m_CheckerboardScale[1]), m_CheckerboardTexture, m_CheckerboardColour, m_TilingFactor[0]);

		Solus::Renderer2D::DrawQuad(glm::vec2(m_Position[0], m_Position[1]), m_Rotation[0], glm::vec2(m_Scale[0], m_Scale[1]), m_squareColour);

		Solus::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	SU_PROFILE_FUNCTION();

	ImGui::Begin("Square Settings");
	ImGui::ColorEdit4("Colour", glm::value_ptr(m_squareColour));
	ImGui::SliderFloat2("Position", m_Position, -5, 5, "%.3f", 1.0f);
	ImGui::SliderFloat2("Scale", m_Scale, 0, 5, "%.3f", 1.0f);
	ImGui::SliderFloat("Roation", m_Rotation, 0, 360, "%.3f", 1.0f);
	ImGui::End();

	ImGui::Begin("Checkerboard Settings");
	ImGui::ColorEdit4("Colour", glm::value_ptr(m_CheckerboardColour));
	ImGui::SliderFloat2("Position", m_CheckerboardPosition, -5, 5, "%.3f", 1.0f);
	ImGui::SliderFloat2("Scale", m_CheckerboardScale, 0, 10, "%.3f", 1.0f);
	ImGui::SliderFloat("Roation", m_CheckerboardRotation, 0, 360, "%.3f", 1.0f);
	ImGui::SliderFloat("TilingFactor", m_TilingFactor, 0, 10, "%.3f", 1.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(Solus::Event& event)
{
	m_CameraController.OnEvent(event);
}