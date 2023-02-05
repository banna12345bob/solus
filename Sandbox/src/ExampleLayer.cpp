#include "ExampleLayer.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
	: Layer("Example"), m_CameraController(1280.0f / 720.0f)
{
	m_Texture = Solus::Texture2D::Create("assets/textures/Checkerboard.png");
	m_OakLogTex = Solus::Texture2D::Create("assets/textures/Oak_Log.png");
}

void ExampleLayer::OnUpdate(Solus::Timestep time)
{
	m_CameraController.OnUpdate(time);

	Solus::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
	Solus::RenderCommand::Clear();

	Solus::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Solus::Renderer2D::DrawQuad(glm::vec3(0.0f), 0, glm::vec2(1.0f), m_OakLogTex);

	Solus::Renderer2D::DrawQuad(glm::vec3(0.0f), 0, glm::vec2(1.5f), m_Texture);

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			glm::vec3 pos(x * 0.11f + 0.5f, y * 0.11f + 0.5f, 0.0f);
			Solus::Renderer2D::DrawQuad(pos, 0, glm::vec2(0.1f), m_squareColour);

		}
	}

	Solus::Renderer2D::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Colour", glm::value_ptr(m_squareColour));
	ImGui::End();
}

void ExampleLayer::OnEvent(Solus::Event& event)
{
	m_CameraController.OnEvent(event);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}
