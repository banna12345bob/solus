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
	
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Solus::Timestep time)
{
	m_CameraController.OnUpdate(time);

	Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
	Solus::RenderCommand::Clear();

	Solus::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Solus::Renderer2D::DrawQuad(glm::vec2(m_Position[0], m_Position[1]), m_Rotation[0], glm::vec2(m_Scale[0], m_Scale[1]), m_squareColour);

	Solus::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Square Settings");
	ImGui::ColorEdit4("Colour", glm::value_ptr(m_squareColour));
	ImGui::SliderFloat2("Position", m_Position, -5, 5, "%.3f", 1.0f);
	ImGui::SliderFloat2("Scale", m_Scale, 0, 5, "%.3f", 1.0f);
	ImGui::SliderFloat("Roation", m_Rotation, 0, 360, "%.3f", 1.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(Solus::Event& event)
{
	m_CameraController.OnEvent(event);
}